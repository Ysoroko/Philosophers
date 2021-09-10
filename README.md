# Philosophers
"Multithreading with a touch of philosophy"

#### N philosophers are sitting around a table with a spaghetti bowl in the middle
#### There is a fork between every pair of philosophers and 2 forks are required to eat
#### Each philosopher will try to grab 2 forks (if available), then eat, then sleep, then think
#### [eat_milliseconds] are required to eat, [sleep_milliseconds] are required to sleep and after [die_milliseconds] without eating a philosopher will die.
#### Each philosopher is a thread and every fork is a mutex.
#### Every change of state of a philosopher is displayed on standard output, along with the time when the change occurs

## Try it out:
-1) Git clone this repository
-2) While inside the repository in the terminal run the command "Make" to create the executable "philo"
-3) Launch the executable with the following arguments:
- ./philo [n_philosophers] [time_to_die_in_ms] [time_to_eat_in_ms] [time_to_sleep_in_ms]
- An optionnal 5th argument [n_times_to_eat] can be provided. With it, after each philosopher has eaten [n_times_to_eat] the simulation will stop.

⚠️ Note: The project is very depending on the environement and its ressources. They can induce a delay in thread execution which might invalid the expected behaviour.

With a high number of philosophers, very low times to eat/sleep or/and a low difference in time between between time_to_die and time_to_eat + time_to_sleep due to this delay philosophers may die when not supposed to.


## Useful ressources:
[Amazing videos which explain threads in C](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
