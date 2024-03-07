Particle System Simulator - README
Tai Kilpatrick - V00943861

Completion Status
Implemented Features:
    [4] Particle system
		Particle system working as normal
    [4] Script commands
		All commands implemented
    [3] Forward Euler
		Forward Euler implemented
    [3] Verlet
		Verlet approximation implemented
    [3] Symplectic Euler
		Symplectic Euler implemented
    [4] Ground collision penalty forces
		Ground collision working
    [1] Gravity
		Gravity added and adjustable
    [1] Locking of a particle’s position
		Particle locking functional
    [2] Particle and spring drawing
		Particles and springs drawn

Commands
Particle System Commands:

    system <sys_name> dim <Number of Particles>:
        Initializes the particle system to hold up to the given number of particles.
        Particles may be initialized to the origin of the world or added later.

    system <sys_name> particle <index> <mass> <x y z vx vy vz>:
        Sets the position, mass, and velocity for a given particle.

    system <sys_name> all_velocities <vx vy vz>:
        Sets the velocity of all particles.

Simulator Commands:

    simulator <sim_name> link <sys name> <Number of Springs>:
        Links the simulator to a particular particle system and initializes it to work with a given number of springs.

    simulator <sim_name> spring <index1> <index2> <ks> <kd> <restlength>:
        Sets up a given spring.
        If the rest length is negative, the system automatically sets it to the distance between the corresponding particles at the time of the command.

    simulator <sim_name> fix <index>:
        Nails particle <index> to its current position.

    simulator <sim_name> integration <euler|symplectic|verlet> <time step>:
        Changes the integration technique used by the given simulator and sets the time step of the integration.

    simulator <sim_name> ground <ks> <kd>:
        Sets the parameters of the penalty forces applied to particles that try to go underground.

    simulator <sim_name> gravity <g>:
        Sets the acceleration due to gravity, in unit length per unit time squared.

    simulator <sim_name> drag <kdrag>:
        Sets the global drag (friction) coefficient (Fi = -kdrag vi).
        Expects a positive number.