*This project has been created as part of the 42 curriculum by urborgscavenger.*

# Philosophers

## Description

This project implements the classic Dining Philosophers problem, a fundamental synchronization problem in computer science. The problem involves a group of philosophers sitting at a round table, each with a plate of spaghetti and a fork between them. To eat, a philosopher needs to pick up both adjacent forks, but there are only as many forks as philosophers. The challenge is to design a solution that allows philosophers to eat, think, and sleep without causing deadlocks or starvation.

The implementation uses POSIX threads (pthreads) and mutexes to manage concurrent access to shared resources (forks). Each philosopher is represented by a thread that cycles through eating, sleeping, and thinking states, while a monitor thread ensures no philosopher dies from starvation.

## Instructions

### Compilation

To compile the project, navigate to the `philo` directory and run:

```bash
make
```

This will generate the executable `philo`.

### Execution

Run the program with the following arguments:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers`: The number of philosophers and forks (must be between 1 and 200).
- `time_to_die`: Time in milliseconds after which a philosopher dies if they haven't started eating.
- `time_to_eat`: Time in milliseconds it takes for a philosopher to eat.
- `time_to_sleep`: Time in milliseconds a philosopher spends sleeping.
- `number_of_times_each_philosopher_must_eat`: Optional argument specifying how many times each philosopher must eat before the simulation stops.

### Example

```bash
./philo 5 800 200 200 7
```

This runs a simulation with 5 philosophers, where each philosopher dies after 800ms without eating, takes 200ms to eat, sleeps for 200ms, and the simulation stops after each philosopher has eaten 7 times.

### Cleaning

To clean object files and the executable:

```bash
make fclean
```

## Resources

AI was mostly asked about deadlocks, threads, processes, and related synchronization concepts to understand and implement the solution.