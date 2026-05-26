import numpy as np 

#to implement a function that computes end effector position of a 2 dof planar arm given joint angles 
#input l1,l2 (link lengths), theta1 ,theta2 (joint angles in radians)
#output (x,y) 

def forward_kinematics(l1,l2,theta1,theta2):

    x = l1*np.cos(theta1) + l2*np.cos(theta2 + theta1)
    y = l1*np.sin(theta1) + l2*np.sin(theta2 + theta1)
    return x,y 

print(forward_kinematics(1.0,1.0,0,0))
