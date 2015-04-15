/* packet single-char codes:
	[2-9] - card (#)
	T - card 10
	J - card 11 (jack)
	Q - card 12 (queen)
	K - card 13 (king)
	A - card 1 (ace)
*/

unsigned char getCardSymbol(unsigned char card) {
	switch (card) {
		case 1:
			return 'A';
		case 10:
			return 'T';
		case 11:
			return 'J';
		case 12:
			return 'Q';
		case 13:
			return 'K';
		default:
			// to translate a base-10 digit to a char, simply add 0x30 to its value
			return card + 0x30;
	}
}

unsigned char getCardValue(unsigned char card) {
	switch (card) {
		case 'A':
			return 1;
		case 'T':
			return 10;
		case 'J':
			return 11;
		case 'Q':
			return 12;
		case 'K':
			return 13;
		default:
			// to translate a char to a base-10 digit, simply subtract 0x30
			return card - 0x30;
	}
}
