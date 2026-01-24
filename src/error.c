int errorHandler(Display* _, XErrorEvent* ev)
{
	printf("X-ERROR --- request %d, error %d\n", ev->request_code, ev->error_code);
	return 0;
}
