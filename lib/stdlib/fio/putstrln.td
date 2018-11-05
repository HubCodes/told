use fio;
use string;

def putstrln(str: *char): int {
  var length: int = strsize(str);
  putstr(str, length);
  putc('\n');
  return length;
}