package brigero;

public enum MessageType {
    NULL(-1),
    NONE(0), // No valid message
    IMU(1), // An IMU message
    POINTCLOUD(2), // A cached PointCloud message
    RANGE(3), // A Range message
    AUXILIARY(4), // An Auxiliary message
    VERSION(5), // A Version info
    TIMESYNC(6); // A time sync info

    public final int t;

    MessageType(int t) {
        this.t = t;
    }
}
