/**
 * reference: https://www.ibm.com/developerworks/cn/linux/l-async/
 */

/**
 * > echo "hello-world" > file.txt
 * > ./aio_test
 * [root@be74e79168dd]~/sdk/builder/io_model# ./aio_test
 * #sizeof(aiocb) == 168 bytes, open fd = 3
 * #count = 109494, tmp_rc = 0 <Success>, EINPROGRESS...
 * #read 0 bytes! <hello>
 * #read 5 bytes! <hello>
 */

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <fcntl.h>
#include <cerrno>
#include <unistd.h>
#include <aio.h>

void test_aio_sample() {
	const int BUFSIZE = 5;
	int fd;
	struct aiocb aio_cb_ptr;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0) {
		perror("fail open");
		exit(1);
	}
	printf("sizeof(aio_cb) == %ld bytes, open fd = %d \n", sizeof(struct aiocb),
			fd);
	/* Zero out the aiocb structure (recommended) */
	memset((void*) &aio_cb_ptr, 0, sizeof(struct aiocb));

	/* Allocate a data buffer for the aiocb request */
	void *buffer = malloc(BUFSIZE + 1);
	if (!buffer) {
		perror("fail malloc");
		exit(1);
	}
	aio_cb_ptr.aio_buf = buffer;

	/*1 Initialize the necessary fields in the aiocb */
	aio_cb_ptr.aio_fildes = fd;
	aio_cb_ptr.aio_nbytes = BUFSIZE;
	aio_cb_ptr.aio_offset = 0;

	int rc = aio_read(&aio_cb_ptr);
	if (rc < 0) {
		perror("aio_read");
		exit(1);
	}

	//2 采用 aio_error() 不断询问，异步任务是否完成
	int count = 0;
	int tmp_rc = 0;
	do {
		++count;
		tmp_rc = aio_error(&aio_cb_ptr);
	} while (tmp_rc == EINPROGRESS);

	printf("count = %d, tmp_rc = %d <%s>, EINPROGRESS...\n", count, tmp_rc,
			strerror(tmp_rc));
	//3 此时异步操作已经完成
	printf("read %ld bytes! errno = %d, data = <%s>\n",
			aio_cb_ptr.__return_value, aio_cb_ptr.__error_code,
			(char*) aio_cb_ptr.aio_buf);

	//4 只是获取异步操作的返回值，因为返回值存储于 aiocb 内部私有成员，应该通过接口进行访问
	rc = aio_return(&aio_cb_ptr);
	if (rc >= 0) {
		/* got ret bytes on the read */
		printf("has read %d bytes! <%s>\n", rc, (char*) aio_cb_ptr.aio_buf);
	} else {
		/* read failed, consult errno */
		printf("fail to read! rc = %d.\n", rc);
	}
	printf("\n===\n");
}

void test_aio_suspend() {
	const int BUFSIZE = 20;
	int fd;
	struct aiocb aio_cb_ptr;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0) {
		perror("fail open");
		exit(1);
	}
	printf("sizeof(aio_cb) == %ld bytes, open fd = %d \n", sizeof(struct aiocb),
			fd);
	/* Zero out the aiocb structure (recommended) */
	memset((void*) &aio_cb_ptr, 0, sizeof(struct aiocb));

	/* Allocate a data buffer for the aiocb request */
	void *buffer = malloc(BUFSIZE + 1);
	if (!buffer) {
		perror("fail malloc");
		exit(1);
	}
	aio_cb_ptr.aio_buf = buffer;

	/*1 Initialize the necessary fields in the aiocb */
	aio_cb_ptr.aio_fildes = fd;
	aio_cb_ptr.aio_nbytes = BUFSIZE;
	aio_cb_ptr.aio_offset = 0;


	//1 将一个异步操作放入list
	const int MAX_LIST = 5;
	struct aiocb *cblist[MAX_LIST];
	printf("sizeof(cblist) = %ld, == 5 * 8 bytes \n", sizeof(cblist));
	memset(cblist,0, sizeof(cblist));
	cblist[0] = &aio_cb_ptr;

	int rc = aio_read(&aio_cb_ptr);
	if (rc < 0) {
		perror("aio_read");
		exit(1);
	}

	//2 aio_suspend 类似  epoll_wait(), 如果超时为NULL，则在请求完成之前不会唤醒该过程。
	/**
	 * 如果列表中的一个或多个请求已终止，则该函数的返回值为0。除此以外，该函数返回 -1，
	 * 并且errno设置为以下值之一：
	 * - EAGAIN 列表中的请求均未在超时指定的时间内完成。
	 * - EINTR 信号中断了aio_suspend函数。该信号也可能由AIO实现发送，同时发出请求之一终止的信号。
	 * - ENOSYS 未实现aio_suspend函数。
	 */
	printf("call aio_suspend()...\n");
	struct timespec timeout = { 1, 0 }; // 1 seconds
	rc = aio_suspend(cblist, MAX_LIST, (const timespec*)&timeout);

	//3 此时异步操作已经完成
	printf("has read %ld bytes! errno = %d, data = <%s>\n",
				aio_cb_ptr.__return_value, aio_cb_ptr.__error_code,
				(char*) aio_cb_ptr.aio_buf);

	printf("\n===\n");
}

/**
 * AIO 提供了一种方法使用 lio_listio API 函数同时发起多个传输。
 * 这个函数非常重要，因为这意味着我们可以在一个系统调用（一次内核上下文切换）中启动大量的 I/O 操作。
 * 从性能的角度来看，这非常重要，因此值得我们花点时间探索一下。
 * 函数原型： int lio_listio( int mode, struct aiocb *list[], int nent,
                   struct sigevent *sig );
 */
/**
 // lio_listio 函数发起一系列请求
 struct aiocb aiocb1, aiocb2;
 struct aiocb *list[MAX_LIST];

 ...
 // Prepare the first aiocb
 aiocb1.aio_fildes = fd;
 aiocb1.aio_buf = malloc( BUFSIZE+1 );
 aiocb1.aio_nbytes = BUFSIZE;
 aiocb1.aio_offset = next_offset;
 aiocb1.aio_lio_opcode = LIO_READ;

 ...

 bzero( (char *)list, sizeof(list) );
 list[0] = &aiocb1;
 list[1] = &aiocb2;

 ret = lio_listio( LIO_WAIT, list, MAX_LIST, NULL );
 */


/** AIO 结合信号 sigaction，进行通知 **/
#ifdef 0
void setup_io( ... )
{
  int fd;
  struct sigaction sig_act;
  struct aiocb my_aiocb;

  ...

  /* Set up the signal handler */
  sigemptyset(&sig_act.sa_mask);
  sig_act.sa_flags = SA_SIGINFO;
  sig_act.sa_sigaction = aio_completion_handler;


  /* Set up the AIO request */
  bzero( (char *)&my_aiocb, sizeof(struct aiocb) );
  my_aiocb.aio_fildes = fd;
  my_aiocb.aio_buf = malloc(BUF_SIZE+1);
  my_aiocb.aio_nbytes = BUF_SIZE;
  my_aiocb.aio_offset = next_offset;

  /* Link the AIO request with the Signal Handler */
  my_aiocb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  my_aiocb.aio_sigevent.sigev_signo = SIGIO;
  my_aiocb.aio_sigevent.sigev_value.sival_ptr = &my_aiocb;

  /* Map the Signal to the Signal Handler */
  ret = sigaction( SIGIO, &sig_act, NULL );

  ...

  ret = aio_read( &my_aiocb );

}


void aio_completion_handler( int signo, siginfo_t *info, void *context )
{
  struct aiocb *req;


  /* Ensure it's our signal */
  if (info->si_signo == SIGIO) {

    req = (struct aiocb *)info->si_value.sival_ptr;

    /* Did the request complete? */
    if (aio_error( req ) == 0) {

      /* Request completed successfully, get the return status */
      ret = aio_return( req );

    }

  }

  return;
}
#endif


int main(int argc, const char *argv[]) {
	test_aio_sample();
	test_aio_suspend();
	return 0;
}
