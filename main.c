int addtwo(int a, int b)
{
	return a+b;
}

int countfive(int a, int b, int c, int d, int e)
{
	return a+(b*c)+d-e;
}

void reset_handler(void)
{
	addtwo(1,2);
	countfive(1,2,3,4,5);
	while (1)
		;
}
