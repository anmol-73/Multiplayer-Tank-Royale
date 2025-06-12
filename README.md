# Multiplayer Tank Royale

**About the project**

This is the repo for the project 'MULTIPLAYER TANK ROYALE' under Programming Club of IITM. We made this game across two semesters (2024-25).

---

## Table of Contents

- [Project Description](#project-description)
- [Libraries and Resources Used](#libraries-and-resources-used)
- [Repo Structure](#repo-structure)
- [Usage](#usage)
- [The Team](#the-team)
- [Photo Gallery](#photo-gallery)

---

## Project Description

- The game is a local multiplayer 2-D top down tank shooter. When connected to the same wi-fi as the host running the server, clients can connect using the host's IP Address. 
- You can create rooms which others can then join. In the room, while waiting for other players, you can set your prefered map.
- There 3 options to choose from for both turret type and tank type. These change the style of gampeplay.
- The objective of the game is to get the most kills until the timer runs out.

---
## Libraries and Resources Used

- We used enet for communication between client and server. It is a library that builds reliability and in-order communication on top of UDP.

- We used raylib for the UI and rendering our game.

- We used free assets from itch.io to make the tanks, maps and other objects in game.


---
## Repo Structure

<details>
<summary>

### `src/common`
Contains files used by both client and server.
</summary>

-  `communication`: This folder contains files defining our communication protocol, i.e. how we encode and decode messages, the structs and commands used by client and server etc.

- `maps`: This folder has files that defines a map object, and also contains data about the three maps like dimensions and types of tiles and tile locations.

- `misc`: This folder contains some misc files useful for certain tasks in client and server like thread pools, queues, string manipulation, void pointers, signal handling.

- `physics`: This folder contains code we use to simulate collisions and other interaction between in game objects.

- `game_communication.hpp`: These files help to serialize/deserialize classes and structs used by the game in order to send or receive them properly.

- `game.hpp` and `game_constants.hpp`: These files define the actual working of the systems once the game has started, i.e. how the game state updates over time with player inputs. Constants and stats related to the various game objects also stored here.

</details>
<details>
<summary>

### `src/client`
Contains files used by client only.
</summary>

- `core/dragonlib`: This is our UI library built on top of raylib made by C++ god kosik. Pls explain this dark magic to us humans.

- `components`: This folder contains dragonlib components we use like buttons, text input, etc.

- `core/scene_management`: This folder defines what are 'Scenes' in our game (similar to a webpage in a website). Has functions to load and unload them properly.

- `core/utils/animations`: This folder contains code to define frames, animations and control them (play, stop, etc).

- `core/utils/camera.hpp`: This file has code to ensure proper conversion of vectors and shapes from units in the actual game to relative to screen (camera). It helps us to render properly on devices with different screen sizes too.

- `core/global.hpp`: This is for loading fonts and shaders we use throughout the scenes for rendering.

- `pages`: This folder contains all the ui, logic, communication that takes place in each of the five scenes (splash, lobby, room, game and game over).

- `service_consumer.hpp`: This file provides a framework for communication by the client. It is built on top of enet.

- `main.cpp`: This file runs the main thread of the client application.

</details>
<details>
<summary>

### `src/server`
Contains files used by server only.
</summary>

- `service_provider.hpp`: This file provides a framework for communication by the server. It is built on top of enet.

- `lobby_service_provider`: This folder defines the behaviour of the server when players first connect. It lets players create rooms.

- `room_service_provider`: This folder defines the behaviour of the server when players join a room. It lets players create games.

- `game_service_provider`: This folder defines the behaviour of the server in game.

- `main.cpp`: This file runs the main thread of the server application.

</details>
<details>
<summary>

### `build`
Contains all resources used by client and server.
</summary>

- It contains the game's client and server exectuables and makefiles.
- It also contains resources like enet .dlls, images and shaders.

</details>

---

## Usage
- Just cd into the directory with the make file and run `make`.
    This should create the executable in the build directory
 
- cd into the build directory and run the game. 

- (Or just run test.bat)

- NOTE: The makefile expects raylib to be installed the normal way (in C:/raylib), if it isn't just change it to fit your usecase ig

- NOTE: This section needs to be changed once we make the distributable zip containing necessary build resources and executable.

---

## The Team

**Project Leads:**
- Anmol Panda
- Jatin V Kumar
- Aditya Palwe

**Project Members:**
- Kaushik G Iyer
- Dhruv Prasad
- Kaushal Raghu
- Shriram Arul

---
## Photo Gallery
![alt text](https://drive.google.com/file/d/1Qhk-I1NP61bTuU41H3lzKzkkMlqR7Xaf/view?usp=drive_link)

![alt text](https://drive.google.com/file/d/1N-Iva0rlnp6MbkzRBfEFxOedrwEhIDy7/view?usp=drive_link)

![alt text](https://drive.google.com/file/d/1eAYNYbh4xHdOFx9xlGBrEvGVhDNUvaqW/view?usp=drive_link)

---
