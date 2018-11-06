use fio;

def getstrln(buffer: *char): int {
  var temp: *char = "  ";
  var index: int = 0;
  getstr(temp, 1);
  for (temp[0] ^ '\n') {
    buffer[index] = temp[0];
    index = index + 1;
    getstr(temp, 1);
  }
  buffer[index] = 0;
  return index;
}