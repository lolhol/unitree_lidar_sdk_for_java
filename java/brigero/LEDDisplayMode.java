package brigero;

public enum LEDDisplayMode {
    FORWARD_SLOW(2),
    FORWARD_FAST(3),
    REVERSE_SLOW(4),
    REVERSE_FAST(5),
    TRIPLE_FLIP(6),
    TRIPLE_BREATHING(7),
    SIXSTAGE_BREATHING(8);

    public final int value;
    LEDDisplayMode(int value) {
        this.value = value;
    }
}
