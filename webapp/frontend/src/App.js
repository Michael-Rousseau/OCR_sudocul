import './App.css';
import Cube from "./components/Cube";
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faGithub} from "@fortawesome/free-brands-svg-icons";
import photo_michael from "./michael.png";
import photo_joanne from "./joanne.jpg";
import photo_arthur from "./arthur.jpeg";
import photo_roman from "./roman.jpg";

function App() {
    const cube_size = 500;

    return (
        <div className="App">
            <div className={"main-container"}>
                <main className="main">
                    <div className={"spinning-cube"}>
                        <div style={{ width: cube_size, height: cube_size }}>
                            <Cube />
                        </div>
                    </div>
                    <div className={"main-content"} >
                        <div className={"project-name"}>
                            <h2 className={"title-sudo"}>SUDO</h2>
                            <h2 className={"title-cul"}>- CUL</h2>
                            <h4 className={"subtitle"}>An AI powered sudoku solver.</h4>
                        </div>
                        <nav className={"nav"}>
                            <a href={"https://github.com/Michael-Rousseau/OCR_Sudocul"} className="github-button nav-button" target="_blank" rel="noopener noreferrer">
                                <FontAwesomeIcon size={'2xl'} icon={faGithub} /><div>View on GitHub</div>
                            </a>
                        </nav>
                    </div>
                </main>
            </div>
            <div className={"about-us"}>
                <h1>About us</h1>
                <div className={"members"}>
                    <div className={"person"}>
                        <div className={"photo"}><img src={photo_michael} alt={"photo"} width={280} height={280}/></div>
                        <div className={"name"}><h1>MICHAEL</h1></div>
                        <div className={"description"}>Junior developper loving to coding on low level</div>
                    </div>
                    <div className={"person"}>
                        <div className={"photo"}><img src={photo_joanne} alt={"photo"} width={280} height={280}/></div>
                        <div className={"name"}><h1>JOANNE</h1></div>
                        <div className={"description"}>A professionnel of images and SDL.</div>
                    </div>
                    <div className={"person"}>
                        <div className={"photo"}><img src={photo_roman} alt={"photo"} width={280} height={280}/></div>
                        <div className={"name"}><h1>ROMAN</h1></div>
                        <div className={"description"}>UI master.</div>
                    </div>
                    <div className={"person"}>
                        <div className={"photo"}><img src={photo_arthur} alt={"photo"} width={280} height={280}/></div>
                        <div className={"name"}><h1>ARTHUR</h1></div>
                        <div className={"description"}>turbo backend engineer</div>
                    </div>
                </div>
            </div>
        </div>
    );
}

export default App;
