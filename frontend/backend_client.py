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
    def __init__(self, command, log_path="backend_log.txt", cwd=None):
        #  cwd      - folder the backend runs in. The C++ backend opens lib/... paths
                      #relative to this, so pass the steam folder. None = current folder.
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
                 cwd=cwd,  
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
    
    USE_FAKE = False   # True = test against fake_backend.py; False = the real C++ backend

    here = Path(__file__).resolve().parent   # the folder this file is in
    steam_dir = here.parent                  # ASSUMES backend.py is in steam/<subfolder>/; adjust if not

    if USE_FAKE:
        b = Backend([sys.executable, "-X", "utf8", str(here / "fake_backend.py")],
                    log_path=str(here / "backend_log.txt"))
        id_a, id_b = 1654170, 815170           # Agony VR, Zombie World (fake data)
        term = "zombie"
    else:
        b = Backend([str(steam_dir / "steam_backend.exe")],
                    log_path=str(here / "backend_log.txt"),
                    cwd=str(steam_dir))        # so lib/... paths resolve
        id_a, id_b = 2386620, 1116520          # Labyrinth, The Backrooms (real data, n = 500)
        term = "labyrinth"

    print("ready:", b.games, "games,", b.edges, "edges")
    print("algos:", b.algos())
    print(f"search '{term}':", b.search(term))
    print(f"search '{term.upper()}':", b.search(term.upper()))   # case-insensitive check

    for algo in b.algos():                     # runs every algorithm the backend reports
        p = b.path(algo, id_a, id_b)
        if p["found"]:
            print(f"{algo} path:", " -> ".join(node["title"] for node in p["path"]),
                  f"({len(p['edges'])} edges, {p['time_us']} us, {p['visited']} visited)")
        else:
            print(f"{algo}: no path ({p['time_us']} us, {p['visited']} visited)")

    same = b.path("bfs", id_a, id_a)
    print("same-game path:", len(same["path"]), "node,", len(same["edges"]), "edges")

    # Each of these must raise BackendError, and the backend must keep running afterwards.
    bad_requests = [
        ("empty search", lambda: b.search("")),
        ("bad algorithm", lambda: b.path("xyz", id_a, id_b)),
        ("unknown id", lambda: b.path("bfs", 1, id_b)),
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
 