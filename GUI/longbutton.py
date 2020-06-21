from PyQt5.QtWidgets import QPushButton

class LongButton(QPushButton):
    def __init__(self, *args, **kwargs):
        QPushButton.__init__(self, *args, **kwargs)
        self.setAutoRepeat(True)
        self.setAutoRepeatDelay(500)
        self.setAutoRepeatInterval(1000)
        self.clicked.connect(self.handleClicked)
        self._state = 0

    def handleClicked(self):
        if self.isDown():
            if self._state == 0:
                self._state = 1
                self.setAutoRepeatInterval(50)
        elif self._state == 1:
            self._state = 0
            self.setAutoRepeatInterval(1000)