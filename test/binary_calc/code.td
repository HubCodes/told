use fio;
use string;
use memory;

def main(): int {
  var buffer: *char = balloc(1);
  var op: *char = "  ";
  var op1: int;
  var op2: int;

  putstrln("Input number 1 >");
  getstrln(buffer);
  op1 = strtoint(buffer);

  putstrln("Input operator >");
  getstrln(buffer);
  op = buffer[0];

  putstrln("Input number 2 >");
  getstrln(buffer);
  op2 = strtoint(buffer);

  puti(op1);
  putc(op);
  puti(op2);
  putstrln("=");

  if (op ~ '+') {
    puti(op1 + op2);
  } else if (op ~ '-') {
    puti(op1 - op2);
  } else if (op ~ '*') {
    puti(op1 * op2);
  } else if (op ~ '/') {
    puti(op1 / op2);
  }
  putc('\n');
  bfree(buffer, 1);
  return 0;
}