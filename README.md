# Block Smashies

Welcome to **Block Smashies**, an exciting and modern twist on the classic brick-breaking genre, featuring a compelling sci-fi storyline and dynamic gameplay.

Block Smashies is a modern take on the classic Arkanoid-inspired brick-breaking game. Developed using the C programming language, it leverages libraries such as Raylib for graphics and Parson for JSON handling, among others. The game is designed to be lightweight, efficient, and extendable, making it a great project for both development and gameplay enthusiasts.

## Story

In a distant future, an unknown alien race composed of nanites—microscopic machines acting as a collective hive mind—threatens the very fabric of the universe. These nanites consume everything in their path, converting entire planets and stars into mysterious, nearly indestructible structures and blocks. These blocks serve as the nanites' fuel and energy sources, forming the foundation of their efforts to expand and ultimately conquer the cosmos.

Despite the overwhelming power of this alien race, an alliance of the galaxy's most advanced species, united under the banner of the United Universe Protectorate (UUP), has developed a weapon capable of combating this menace: the **Orb Weapon**. This powerful device can destroy the nanite blocks on contact, but there is a significant challenge—only one Orb exists, and once released, it repels off everything it touches, making it extraordinarily difficult to control.

For decades, the Orb Weapon remained dormant in orbit around a distant planet’s moon, a relic of a past war, until a brilliant UUP scientist discovered a way to harness its power. The scientist developed a cutting-edge ship designed to be the ultimate weapon against the nanites. This ship, known as the **Protectorate Vessel**, is a marvel of engineering, capable of splitting into up to four modular segments, each controlled by a different pilot. These segments work in unison, allowing for precise control and coordination in guiding the Orb.

The ship is equipped with **containment discs**, advanced technology that can be deployed to create a containment field around the play area, ensuring the Orb remains within a controlled space. These discs emit powerful energy beams that box in the Orb, simulating the classic breakout-style gameplay but in the vastness of space. However, the energy required for this containment leaves one side—directly behind the ship—vulnerable, adding a critical layer of strategy as pilots must ensure the Orb doesn't escape.

To further aid in controlling the Orb, the ship is fitted with **catch modules**. These modules are designed to intercept and recapture the Orb when it would otherwise be lost in space, returning it to the containment area. However, these modules are fragile and can only be used a limited number of times before they are destroyed, representing the ship's finite resources and the player's lives.

As the Protectorate Vessel battles through the alien structures, the ship's segments work together to guide the Orb through the nanite blocks, strategically deploying the containment discs and utilizing the catch modules. The pilots must coordinate their efforts, making split-second decisions to navigate the Orb through increasingly complex networks of blocks. Each successful destruction of a block weakens the nanites' hold on the universe, but with every mission, the stakes grow higher, and the nanites' defenses become more formidable.

The fate of the universe rests on the pilots' ability to master the Orb, use their ship's modular capabilities wisely, and outmaneuver the relentless nanite threat. In this high-stakes battle for survival, the UUP's finest must race against time to shatter the alien network and save the cosmos from annihilation.

## Features

- **Classic Gameplay:** Relive the excitement of brick-breaking with a modern sci-fi twist.
- **Configurable Settings:** Customize your gameplay experience through various settings and options.
- **Lightweight and Fast:** Built with performance in mind, utilizing C language efficiency and optimized libraries.
- **Cross-Platform Compatibility:** Designed to run on multiple platforms, ensuring a broad audience reach.

### Unique Gameplay Features

Take your gaming experience to the next level with these advanced features:

- **Collectible Nanites:** Destroying a block will release collectible nanites, which can be used to develop new technologies and upgrade your ship or orb weapon.
  
- **Orb Retrieval and Upgrades:** After each mission within an operation, the Orb Weapon can be rebounded into a controlled orbit, allowing the ship to dock at a station for upgrades. During this time, you can choose from various upgrades and perks using the collected nanites.
  
- **Ship and Orb Upgrades:**
  - **Speed Boost:** Increases the ship's movement speed.
  - **Thrusters:** Enhances the ship's maneuverability, allowing for quicker direction changes and stops.
  - **Boosters:** Double-tap in a direction to perform a quick burst of speed.
  - **Shield Extension:** Expands the shield, making the shield surface larger.
  - **Shield Pulse:** Time your rebound with a button press to increase the Orb Weapon's speed.
  - **Detachable Power Modules:** Permanently increase the Orb Weapon’s power, changing its visual appearance as it becomes more destructive.
  
- **Advanced Tech Upgrades and Perks:**
  - **Phase Shift:** Chance to create a time-shifted duplicate of the Orb Weapon that lasts for a limited time after rebounding off the ship.
  - **Phase Nova:** A chance for the Orb Weapon to unleash an area-of-effect explosion upon hitting a block, potentially destroying multiple blocks at once.
  - **Super Nova:** Chance for the Orb Weapon to transform into an unstoppable force after rebounding, smashing through all blocks and only rebounding off the ship and walls for a limited time.
  - **Time Manipulation:** Use booster energy to slow down time, allowing for more precise control over the Orb Weapon's rebounds.
  - **Orb Shot:** Use booster energy to fire mini versions of the Orb Weapon like a machine gun from the ship; these mini-orbs destroy on impact.
  
- **Challenging Boss Fights:** Each operation consists of multiple missions, with the final mission featuring a unique boss encounter. Bosses can:
  - **Fast Rebounds:** Increase the speed of the Orb Weapon to make it harder to control.
  - **Enemy Spawning:** Summon enemies that can hinder the ship or cause negative effects on contact.
  - **Timed Challenges and Puzzles:** Incorporate block-based puzzles and timed events that must be solved to defeat the boss.
  - **Shifting Blocks:** Rearrange blocks during the fight to create unpredictable setups, adding a layer of strategy to the gameplay.

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
- **box2d:**  A popular physics engine for simulating 2D rigid body physics. In Block Smashies, it is used to handle the physical interactions between the Orb Weapon, blocks, and other dynamic objects, providing realistic collision detection, momentum, and bounce effects that enhance the gameplay experience.
- **klib:** Specifically used for kvec, a dynamic array implementation in C. kvec provides a simple yet powerful way to manage dynamic arrays, which are used in Block Smashies to efficiently store and manipulate collections of game elements like entities, scores, and other data that require dynamic resizing.
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

**Debian/Ubuntu Build Dependencies**
   ```bash
  sudo apt install build-essential git libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev cmake ninja-build
   ```

## Contributing

Contributions are welcome! If you have ideas for new features, improvements, or bug fixes, feel free to submit a pull request or open an issue.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
