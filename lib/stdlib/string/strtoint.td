use string;
use fio;

def strtoint(string: *char): int {
  var result: int = 0;
  var length: int = strsize(string);
  var origin_length: int = length;
  var index: int = 0;
  var factor: int = 1;
  for (length > 1) {
    factor = factor * 10;
    length = length - 1;
  }
  for (index < origin_length) {
    result = result + (string[index] - 48) * factor;
    factor = factor / 10;
    index = index + 1;
  } 
  return result;
}