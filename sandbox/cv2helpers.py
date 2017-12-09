import cv2

KEY_NEWLINE = 10
KEY_ENTER = 13
KEY_ESC = 27
KEY_NUMBERS = range(48, 58)
KEY_PERIOD = 46
KEY_MINUS = 45

_NUM_MAP = {k: str(k-48) for k in KEY_NUMBERS} # convert key codes to number values
_NUM_MAP[KEY_PERIOD] = '.'
_NUM_MAP[KEY_MINUS] = '-'


def keys_to_float(keys):
	return float(''.join(map(str, keys)))

def awaitKeys(keys, delay_ms):
	if KEY_ENTER in keys:
		keys.append(KEY_NEWLINE)
	while 1:
		# print "awaiting keys", keys
		retkey = (cv2.waitKey(delay_ms) & 0xEFFFFF)
		# print "got key", retkey
		if retkey in keys:
			if retkey == KEY_NEWLINE:
				return KEY_ENTER
			return retkey


def awaitNumber():

	keypresses = []
	sign = 1
	while 1:
		retkey = awaitKeys([KEY_PERIOD, KEY_MINUS, KEY_NEWLINE, KEY_ENTER, KEY_ESC] + KEY_NUMBERS, 0)
		# print retkey
		if retkey in (KEY_NEWLINE, KEY_ENTER):
			return sign * keys_to_float(keypresses)

		elif retkey in KEY_NUMBERS + [KEY_PERIOD]:
			if KEY_PERIOD not in keypresses:
				keypresses.append(_NUM_MAP[retkey])
				print sign * keys_to_float(keypresses)

		elif retkey == KEY_MINUS:
			sign *= -1
			if len(keypresses):
				print sign * keys_to_float(keypresses)

		elif retkey == KEY_ESC:
			return KEY_ESC
