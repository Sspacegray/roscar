import numpy as np
import matplotlib.pyplot as plt

class DifferentialDriveRobot:
   def __init__(self, wheel_radius=0.05, wheel_base=0.15, dt=0.1):
       self.R = wheel_radius  # 轮子半径（米）
       self.L = wheel_base    # 两轮间距（米）
       self.dt = dt           # 时间步长
       self.x = 0.0           # 机器人x位置
       self.y = 0.0           # 机器人y位置
       self.theta = 0.0       # 机器人朝向角度

   def set_speed(self, v, omega):
       # 计算左右轮速度
       omega_l = (2 * v - omega * self.L) / (2 * self.R)
       omega_r = (2 * v + omega * self.L) / (2 * self.R)
       return omega_l, omega_r

   def update_position(self, omega_l, omega_r):
       # 更新速度
       v = self.R * (omega_l + omega_r) / 2
       omega = self.R * (omega_r - omega_l) / self.L

       # 更新位置
       self.x += v * np.cos(self.theta) * self.dt
       self.y += v * np.sin(self.theta) * self.dt
       self.theta += omega * self.dt

   def simulate(self, v, omega, steps):
       # 存储位置轨迹
       x_path = [self.x]
       y_path = [self.y]

       for _ in range(steps):
           omega_l, omega_r = self.set_speed(v, omega)
           self.update_position(omega_l, omega_r)
           x_path.append(self.x)
           y_path.append(self.y)

       return x_path, y_path

# 初始化机器人
robot = DifferentialDriveRobot()

# 定义线速度和角速度
v = 0.1    # 线速度 (m/s)
omega = 0.5  # 角速度 (rad/s)
steps = 100  # 模拟步数

# 模拟并绘制运动轨迹
x_path, y_path = robot.simulate(v, omega, steps)
plt.plot(x_path, y_path, label="Robot Path")
plt.xlabel("X position (m)")
plt.ylabel("Y position (m)")
plt.legend()
plt.show()