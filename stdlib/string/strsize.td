def strsize(str: *char): int {
	var i: int = 0;
	for (str[i] ^ 0) {
		i = i + 1;
	}
	return i;
}
