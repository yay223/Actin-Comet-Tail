
Defining a quaternion as the complex sum of a scaler and vector, we can write the quaternion as: 
$$ \tilde{q} = q_4 + \hat{i}q_1 + \hat{j}q_2 + \hat{k}q_3 = q_4 + i \bold{q} $$

The product of two quaternions is given by [1]:
$$\begin{align} \tilde{q} \tilde{p} = (q_4 \bold{p} - \bold{q}  \cdot \bold{p}) + i(q_4 \bold{p} + \bold{q}p_4 + \bold{q} \times \bold{p}) \end{align}$$

Which gives the components of quaternion product that are used in the code: 
$$\begin{align} 
\begin{cases}
Q1 = q_4p_1 + q_1p_4 + q_2p_3 - q_3p_2 \\
Q2 = q_4p_2 + q_2p_4 - q_1p_3 + q_3p_1 \\
Q3 = q_4p_3 + q_3p_4 + q_1p_2 - q_2p_1 \\
Q4 = q_4p_4 - q_1p_1 - q_2p_2 - q_3p_3 
\end{cases}
\end{align}$$

## Relation between angular velocity in cartesian coordinates to quaternions [1]

$$\begin{align}
\begin{bmatrix}
\omega_x^{\prime}\\
\omega_y^{\prime}\\
\omega_z^{\prime}\\
\end{bmatrix} = 2W\begin{bmatrix}
\dot{q_1}\\
\dot{q_2}\\
\dot{q_3}\\
\dot{q_4}\\
\end{bmatrix} \end{align}$$

where,
$$\begin{align} W = \begin{bmatrix}
q_4 & q_3 & -q_2 & -q_1\\
-q_3 & q_4 & q_1 & -q_2\\
q_2 & -q_1 & q_4 & -q_3\\
q_1 & q_2 & q_3 & q_4\\
\end{bmatrix} \end{align}$$




-----------
[1] The Art of MD Simulations D.C. Rapport