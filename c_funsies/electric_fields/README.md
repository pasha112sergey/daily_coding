# Electric Field Simulation!

I'm trying to learn C++ and the SDL Library, so I wanted to do a simple project to visualize the electric fields of point charges.

## Brief physics lesson

The electric field vector $\vec{E}$ is defined as:
$
\begin{align}
\vec{E} = \frac{kQ}{r^2}\vec{r}
\end{align}
$
where $\vec{r}$ is the vector of distance between the point at which the field is being evaluated and charged particle creating the field, $k$ is a fundamental constant, and $Q$ is the charge of the particle creating the field.

We can convert the Field to a force by using the following equation:
$
\begin{align}
\vec{F} = q\vec{E}
\end{align}
$
where q is the charge experiencing the force.

After these E&M-specific definitions, we can use the basic Newtonian kinematics to model motion in the system.

## Remarks on Future Code Improvement and Reflection

I'm very new to C++, this was this actually my very first experience programming! I should've applied OOP principles better to the point and FieldVector objects, and just generally made the code more readable.

## Run instructions:

Run with `./bin/fields`. Then, you can move the positive charge by dragging the mouse. Add negative charges by clicking space, and spawn a positive point charge at mouse cursor by clicking 'A'.

## Screenshot :)

![Screenshot](assets/Demo%20Screenshot.png);
