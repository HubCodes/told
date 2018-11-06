use fio;

def puti(data: int): int {
	var ch: int = 0;
	var buffer: *char = "\0\0\0\0\0\0\0\0\0\0\0";
	var o_digits: int = 0;
	var digits: int = 0;
	var copied: int = data;
	var index: int = 0;
	var length: int = 0;
	if (data ~ 0) {
		putc('0');
		return 1;
	}
	for (copied ^ 0) {
		digits = digits + 1;
		copied = copied / 10;
	}
	o_digits = digits;
	for (digits ^ 0) {
		index = digits - 1;
		buffer[index] = (data % 10) + 48;
		data = data / 10;
		digits = digits - 1;
	}
	length = o_digits;
	index = 0;
	putstr(buffer, o_digits);
	for (index < length) {
		buffer[index] = 0;
		index = index + 1;
	}
	return digits;
}

