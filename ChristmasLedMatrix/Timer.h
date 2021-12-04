class Timer
{
  public:
    Timer(uint32_t interval);
    void setInterval(uint32_t interval);
    boolean isReady();
    void reset();

  private:
    uint32_t _timer = 0;
    uint32_t _interval = 0;
};

Timer::Timer(uint32_t interval) {
  _interval = interval;
  _timer = millis();
}

void Timer::setInterval(uint32_t interval) {
  _interval = interval;
}

boolean Timer::isReady() {
  if ((long)millis() - _timer >= _interval) {
    _timer = millis();
    return true;
  } else {
    return false;
  }
}

void Timer::reset() {
  _timer = millis();
}
