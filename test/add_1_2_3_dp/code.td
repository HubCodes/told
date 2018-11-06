use fio;
use string;
use memory;

def solve(dp: *int, n: int): int {
  var i: int = 4 * 4;
  dp[1 * 4] = 1;
  dp[2 * 4] = 2;
  dp[3 * 4] = 4;
  for (i < n * 4 | i ~ n * 4) {
    dp[i] = dp[i-(1*4)] + dp[i-(2*4)] + dp[i-(3*4)]; 
    i = i + 4;
  }
  return dp[n*4];
}

def main(): int {
  var i: int = 0;
  var dp: *int = [int]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  var TC: int;
  var buffer: *char = "          ";
  getstrln(buffer);
  TC = strtoint(buffer);
  puti(solve(dp, TC));
  putc('\n');
  return 0;
}
