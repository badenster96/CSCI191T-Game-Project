| --------------------------------- |
| --- Mission: Totally Possible --- |
| --------------------------------- |

Mission: Totally Possible is a third person autoshooter in a similar vein to Vampire Survivors
and MegaBonk. You play as the muscled-up secret agent Smith, and are tasked with clearing out as
many zombies as you can, descending further into the villain's lair with each victory.

Teammates:
Ben Manley - 
	Menu code:
		Wrote menu code, sound file implementation, and the help screen
	Enemies:
		Wrote enemy logic to target closest player, collision detection with other enemies,
		enemy stats, sound file loading, and hit detection with the player
	Player:
		Wrote player item stats, a fileLoader to load items to an inventory, and the
		pickup system, and developed the 3D movement logic so the player moves relative 
		to the camera direction
	Inventory:
		Created an inventory system that is fully modular, with an as of now proken sprite loader
		to display items on screen
	Level:
		Wrote bullet hit detection, penetration, range, and damage calculations all determined
		by player stats, level bounds check and a simple skybox, and a simple fog overlay
	HUD:
		Created a HUD displaying all crucial player stats, normalized from raw floats for
		player accessibility, a changing player health bar, and an in game debug screen, accessed
		by pressing '~'
	Sounds:
		Created modified sounds functions that plays from a list of random sounds, to make each 
		action, like player hit, enemy hit, enemy death, menu sounds, ect. feel more dynamic
	