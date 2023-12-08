import './App.css';

function App() {
  return (
      <div className="App">
          <main className="main">
              <div className={"main-content"} >
                  <div className={"spinning-cube"}>
                      Cube
                  </div>
                  <div className={"project-name"}>
                      <h2>SUDO</h2>
                      <h2>CUL</h2>
                  </div>
              </div>
              <nav className={"nav"}>
                  <button>About us</button>
                  <button>Solve sudoku now !</button>
                  <button>View the project</button>
              </nav>
          </main>
      </div>
  );
}

export default App;
