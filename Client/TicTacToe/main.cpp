#include <stdio.h>
int main(void)
{
	int board[3][3] = { 11, 12, 13, 14, 15, 16, 17, 18, 19 }; // ���� ����, �� ĭ�� ��ȣ
	int player = 0; // player ID 0,1 �θ��� �÷��̾�
	int x, y; // ���� ��ġ
	int choice = 0; // ������ Ƚ��

	int Bingo(int ary[3][3], int n); // ���� ���� �Լ� ����

	while (1) // ���ٸ� ���Ǿ��� ���ѹݺ�
	{
		// ���� ��ġ �Է�
		do
		{
			printf("player [%d]�� ���� ��ġ(x,y)�� �Է��Ͻÿ� : ", player);
			scanf_s("%d %d", &x, &y); // ���� ��ġ �Է¹ޱ�

			if (board[x][y] < 11 || x>2 || x < 0 || y>2 || y < 0) {
				printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n\n");
			} // �߸��� �Է°��� �ޱ����� ���ǹ�

		} while (board[x][y] < 11 || x>2 || x < 0 || y>2 || y < 0); // �Է°��� ��Ȯ�ϸ� ��������

		choice++; // ������ Ƚ�� 1����

		if (player == 0) {
			board[x][y] = 1; // player 0�� �����ǿ� 1�� �Է�
			player = 1; //�÷��̾� ��ȯ
		}

		else {
			board[x][y] = 2; // player 1�� �����ǿ� 2�� �Է�
			player = 0; // �÷��̾� ��ȯ
		}

		printf("\n");

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == 1) {
					printf("%c", 'O'); // ������ ���ڰ� 1�̸� O
				}
				else if (board[i][j] == 2) {
					printf("%c", 'X'); // ������ ���ڰ� 2�̸� X
				}
				else {
					printf("*"); // ������ ���ڸ� *
				}
			}
			printf("\n");
		} // ������ ���

		  // ���� ����
		int result = Bingo(board, choice); // ���� ���� �Լ� ȣ��
		if (result == 0) //���а� �����Ǵ� ���
		{
			if (player == 0) {
				printf("player [1] �¸�!!\n");
			}
			else {
				printf("player [0] �¸�!!\n");
			}
			break;
		} // ���������� �� �÷��̰� �¸��ϰ� ��

		else if (result == 1) // ���ºΰ� �Ǵ� ���
		{
			printf("���º� �Դϴ�.\n");
			break;
		}
		else if (result == 2) // ��⸦ ��� �����ϴ� ���
		{
			printf("\n");
		}
	}
}

int Bingo(int ary[3][3], int n) {

	if ((ary[0][0] == ary[1][1] && ary[0][0] == ary[2][2]) || (ary[0][2] == ary[1][1] && ary[0][2] == ary[2][0])) {
		return 0;
	} //�밢������ ���� ������� Ȯ��

	for (int line = 0; line <= 2; line++) {
		if ((ary[line][0] == ary[line][1] && ary[line][0] == ary[line][2]) || (ary[0][line] == ary[1][line] && ary[0][line] == ary[2][line])) {
			return 0;
		} //����, �����ٷ� ���� ������� Ȯ��, line�� �� ����, �������� �� ��ȣ 
	}

	if (n == 9) {
		return 1;
	} //����Ƚ���� 9���̸� ���ºη� ����

}// ���� ���� �Լ� ����
