package brigero;

public class Point {

  public final float x, y, z, intensity, time;
  public final int ring;

  public Point(
    float x,
    float y,
    float z,
    float intensity,
    float time,
    int ring
  ) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.intensity = intensity;
    this.time = time;
    this.ring = ring;
  }
}
