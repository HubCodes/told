use fio;
use string;

def quick_sort(arr: *int, left: int, right: int): int {
	var i: int = left;
	var j: int = right;
	var pivot: int = arr[((left + right) / 2) * 4];
	var temp: int = 0;
	for (arr[i * 4] < pivot) {
		i = i + 1;
	}
	for (arr[j * 4] > pivot) {
		j = j - 1;
	}
	if (i < j | i ~ j) {
		temp = arr[i * 4];
		arr[i * 4] = arr[j * 4];
		arr[j * 4] = temp;
		i = i + 1;
		j = j - 1;
	}
	for (i < j | i ~ j) {
		for (arr[i * 4] < pivot) {
			i = i + 1;
		}
		for (arr[j * 4] > pivot) {
			j = j - 1;
		}
		if (i < j | i ~ j) {
			temp = arr[i * 4];
			arr[i * 4] = arr[j * 4];
			arr[j * 4] = temp;
			i = i + 1;
			j = j - 1;
		}
	}
	if (left < j) {
		quick_sort(arr, left, j);
	}
	if (i < right) {
		quick_sort(arr, i, right);
	}
}

def main(): int {
	var a: *int = [int]{ 5, 3, 7, 9, 1, 10, 3, 5 };
	var len: int = 8;
	var index: int = 0;
	quick_sort(a, 0, len); 
	putc('{');
	for (index < len * 4) {
		puti(a[index]);
		index = index + 4;
		putc(',');
	}
	putc('}');
	putc('\n');
	return 0;
}
