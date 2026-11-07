import json
import subprocess
import sys
import threading
from pathlib import Path
 
 
class BackendError(Exception):
    """The backend rejected a request and replied with {"error": "..."}."""
 
 
class BackendDiedError(Exception):
    """The backend process exited, or sent something that isn't a JSON line."""
 
 
class Backend:
    def __init__(self, command, log_path="backend_log.txt"):
        
        self._log = open(log_path, "w", encoding="utf-8")
 
       
        self._lock = threading.Lock()
 
        try:
            self.proc = subprocess.Popen(
                command,
                stdin=subprocess.PIPE,    # we write commands here
                stdout=subprocess.PIPE,   # we read JSON replies here
                stderr=self._log,         # backend logs go to the file
                text=True,                # exchange str, not bytes
                encoding="utf-8",         # non-English titles decode correctly on Windows
            )
        except OSError:
            # Raised when the program itself can't be started (e.g. wrong .exe path).
            self._log.close()
            raise
 
        
        ready = self._read_reply()
        if ready.get("status") != "ready":
            self.close()
            raise BackendDiedError(f"expected the ready line, got: {ready}")
 
        self.games = ready.get("games", 0)   # for display in the UI
        self.edges = ready.get("edges", 0)
 
    # ---------- private helpers: all protocol I/O happens here ----------
 
    def _read_reply(self):
        
        line = self.proc.stdout.readline()   # waits until a whole line arrives
        if line == "":
            
            raise BackendDiedError("backend exited unexpectedly; see the log file")
        try:
            return json.loads(line)
        except json.JSONDecodeError:
           
            raise BackendDiedError(f"backend sent non-JSON output: {line!r}")
 
    def _request(self, command):
        """
        Send one command line and return its parsed reply.
        Raises BackendError for {"error": ...} replies.
        """
        with self._lock:
            if self.proc.poll() is not None:   
                raise BackendDiedError("backend is not running")
            try:
                self.proc.stdin.write(command + "\n")   
                self.proc.stdin.flush()                 # push it through the pipe now
            except OSError:
                
                raise BackendDiedError("could not send to the backend; see the log file")
            reply = self._read_reply()
 
        if "error" in reply:
            raise BackendError(reply["error"])
        return reply
 
    
 
    def algos(self):
        
        return self._request("algos")["algos"]
 
    def search(self, text):
        
        clean = text.replace("\r", " ").replace("\n", " ").strip()
        return self._request(f"search {clean}")["results"]
 
    def path(self, algo, from_id, to_id):
        
        return self._request(f"path {algo} {int(from_id)} {int(to_id)}")
 
    def close(self):
        
        if self.proc.poll() is None:   # still running
            try:
                self.proc.stdin.write("quit\n")
                self.proc.stdin.flush()
            except OSError:
                pass                   
            try:
                self.proc.wait(timeout=5)              # give it 5 seconds to exit cleanly
            except subprocess.TimeoutExpired:
                self.proc.kill()                       # then force it
                self.proc.wait()
        if not self._log.closed:
            self._log.close()
 
 
# ---------- self-test: runs only with `python backend.py`, not on import ----------
if __name__ == "__main__":
    
    here = Path(__file__).parent
    b = Backend([sys.executable, "-X", "utf8", str(here / "fake_backend.py")],
                log_path=str(here / "backend_log.txt"))
 
    print("ready:", b.games, "games,", b.edges, "edges")
    print("algos:", b.algos())
    print("search 'zombie':", b.search("zombie"))
    print("search 'VTUBER':", b.search("VTUBER"))
 
    p = b.path("bfs", 1654170, 815170)
    print("bfs path:", " -> ".join(node["title"] for node in p["path"]),
          f"({len(p['edges'])} edges, {p['time_us']} us, {p['visited']} visited)")
 
    same = b.path("bfs", 1654170, 1654170)
    print("same-game path:", len(same["path"]), "node,", len(same["edges"]), "edges")
 
    
    bad_requests = [
        ("empty search", lambda: b.search("")),
        ("bad algorithm", lambda: b.path("xyz", 1654170, 815170)),
        ("unknown id", lambda: b.path("bfs", 1, 815170)),
    ]
    for label, call in bad_requests:
        try:
            call()
            print(f"{label}: NO ERROR (unexpected!)")
        except BackendError as e:
            print(f"{label}: error as expected -> {e}")
 
    print("still alive after errors:", b.algos())
    b.close()
    print("closed; exit code:", b.proc.returncode)
 