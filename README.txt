  // CAN Command Packet Structure ///
  // 16 bit position command, between -4*pi and 4*pi
  // 12 bit velocity command, between -30 and + 30 rad/s
  // 12 bit kp, between 0 and 500 N-m/rad
  // 12 bit kd, between 0 and 100 N-m*s/rad
  // 12 bit feed forward torque, between -18 and 18 N-m
  // CAN Packet is 8 8-bit words
  // Formatted as follows.  For each quantity, bit 0 is LSB
  // 0: [position[15-8]]
  // 1: [position[7-0]] 
  // 2: [velocity[11-4]]
  // 3: [velocity[3-0], kp[11-8]]
  // 4: [kp[7-0]]
  // 5: [kd[11-4]]
  // 6: [kd[3-0], torque[11-8]]
  // 7: [torque[7-0]]



    // CAN Reply Packet Structure ///
  // 16 bit position, between -4*pi and 4*pi
  // 12 bit velocity, between -30 and + 30 rad/s
  // 12 bit current, between -40 and 40;
  // CAN Packet is 5 8-bit words
  // Formatted as follows.  For each quantity, bit 0 is LSB
  // 0: [position[15-8]]
  // 1: [position[7-0]] 
  // 2: [velocity[11-4]]
  // 3: [velocity[3-0], current[11-8]]
  // 4: [current[7-0]]

ParameterRange::ParameterRange(float pMin, float pMax, float vMin, float vMax, float tMin, float tMax, float kpMin, float kpMax, float kdMin, float kdMax)
ParameterRange range(-95.5f, 95.5f, -45.0f, 45.0f, -18.0f, 18.0f, 0.0f, 500.0f, 0.0f, 5.0f);

