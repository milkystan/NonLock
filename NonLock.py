import non_lock
import time


class NonLock:
    def __init__(self):
        self.p = non_lock.new_value()

    def acquire(self, blocking=True):
        if not blocking:
            return non_lock.acquire(self.p)
        while not non_lock.acquire(self.p):
            time.sleep(0)

    def release(self):
        non_lock.release(self.p)

