package brigero;

public class IMUUnitree {

  public double stamp; // imu timestamp
  public int id; // sequence id
  public float[] quaternion; // quaternion: [x,y,z,w]
  public float[] angular_velocity; // three-axis angular velocity values.
  public float[] linear_acceleration; // three-axis acceleration values.

  public IMUUnitree(
    double stamp,
    int id,
    float[] quaternion,
    float[] angular_velocity,
    float[] linear_acceleration
  ) {
    this.stamp = stamp;
    this.id = id;
    this.quaternion = quaternion;
    this.angular_velocity = angular_velocity;
    this.linear_acceleration = linear_acceleration;
  }
}
