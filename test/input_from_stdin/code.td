use fio;
use string;
use memory;

def main(): int {
  var buffer: *char = balloc(1);
  var data: int = 0;
  getstrln(buffer);
  data = strtoint(buffer);
  puti(data);
  putc('\n');
  bfree(buffer, 1);
  return 0;
}