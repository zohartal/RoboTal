
int color_sensor_read16(int i2c_address, char sensor_address);
void color_sensor_write16(int i2c_address, char sensor_address, int data);
void handle_write( int reg, int data);
void handle_read( int reg);