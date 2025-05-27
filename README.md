# DC Motor Speed Control using PID Controller


## 🎯 Objective

To achieve precise speed/position control of a DC motor by minimizing errors, overshoot, and the effects of disturbances using Proportional, Integral, and Derivative adjustments.

---

## 🧠 System Modeling

The DC motor system is modeled with consideration of:

- Electrical & mechanical losses
- Torque: \( T = K \cdot i \)
- Back EMF: \( e = K_c \cdot \dot{\theta} \)
- Mechanical equation: \( J \ddot{\theta} + b \dot{\theta} = K \cdot i \)
- Electrical equation: \( L \frac{di}{dt} + R \cdot i = V - K \cdot \dot{\theta} \)

**Transfer Functions:**
- \( s (Js + b) \Theta(s) = KI(s) \)
- \( (Ls + R) I(s) = V(s) − K_s \Theta(s) \)

---

## 🔍 Parameter Estimation

Accurate estimation of motor parameters is crucial for controller performance:

1. Apply varying voltages to the motor.
2. Measure the corresponding speeds (rad/s).
3. Store data in Excel.
4. Import to MATLAB.
5. Simulate in Simulink to derive motor constants.

---

## 🎛️ PID Controller

The PID controller controls the speed of the motor based on encoder feedback:

- **Kp**: Increases responsiveness (but may cause overshoot).
- **Ki**: Eliminates steady-state error.
- **Kd**: Reduces overshoot and oscillations.

### 📐 Tuning: Ziegler-Nichols Method

- \( K_c \): Ultimate gain (gain at sustained oscillation)
- \( T \): Oscillation period at \( K_c \)
- \( T_i, T_d \): Integral and Derivative time constants

---

## 🧹 Low-Pass Filter Design

To reduce measurement noise:

- IIR Filter design with:
  - \( \alpha = 0.854 \)
  - \( \beta = 0.0728 \)

This filter smooths encoder signals for more accurate control.

---

## 🛠️ Mechanical Design

Design elements include motor mount, encoder setup, and safe mechanical integration for testing.

---

## 📊 Simulation and Real-Time Testing

- Model and control algorithm developed in MATLAB/Simulink
- Real-time control implemented via embedded hardware

---

## 📁 Repository Contents

