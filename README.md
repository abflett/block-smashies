# Block Smashies

Block Smashies is a modern take on the classic Arkanoid-inspired brick-breaking game. Developed using the C programming language, it leverages libraries such as Raylib for graphics and Parson for JSON handling, among others. The game is designed to be lightweight, efficient, and extendable, making it a great project for both development and gameplay enthusiasts.

## Features

- **Classic Gameplay:** Relive the excitement of brick-breaking with a modern twist.
- **Configurable Settings:** Customize your gameplay experience through various settings and options.
- **Lightweight and Fast:** Built with performance in mind, utilizing C language efficiency and optimized libraries.
- **Cross-Platform Compatibility:** Designed to run on multiple platforms, ensuring a broad audience reach.

## Development Status

### To-Do

- **Gameplay:**
  - Implement core gameplay mechanics including paddle movement, ball physics, and brick destruction.
  - Integrate power-ups and different brick types for enhanced gameplay dynamics.
- **Menus and GUI:**
  - Develop interactive menus for navigation, including Start, Options, and High Scores.
  - Implement a configurable options menu using a graphical user interface (GUI).
- **Asset Management:**
  - Integrate PhysFS for efficient virtual file system management, with separate archives for static assets and dynamic data.

## Libraries and Tools Used

- **Raylib:** A simple and easy-to-use library for graphics, used for rendering the game.
- **Parson:** A lightweight JSON library in C for reading and writing configuration files and game settings.
- **PhysFS (Planned):** A library to manage virtual file systems, which will be used for packaging assets and dynamic game data.
- **uthash:** A simple and efficient hash table library for C, used for managing and retrieving resources quickly within the game. It provides fast lookups and insertions, making it ideal for handling in-game assets and settings.

## How to Build

To build Block Smashies, you need to have a C compiler and CMake installed. Follow these steps:

1. Clone the repository:

   ```bash
   git clone --recursive https://github.com/abflett/block-smashies.git
   cd block-smashies

   ```

2. Create a build directory and navigate to it:

   ```bash
   mkdir build
   cd build

   ```

3. Run CMake to configure the project:

   ```bash
   cmake ..

   ```

4. Compile the project:

   ```bash
   cmake --build .

   ```

5. Run the game:
   ```bash
   ./BlockSmashies
   ```

## Contributing

Contributions are welcome! If you have ideas for new features, improvements, or bug fixes, feel free to submit a pull request or open an issue.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
