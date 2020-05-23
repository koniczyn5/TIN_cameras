#include "message.cpp"


#define BOOST_TEST_MODULE MessageTests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/lambda/lambda.hpp>

BOOST_AUTO_TEST_CASE(Message_test)
{
	char id = TEST_ACK;
	char arr[10] = "test12345";
	char arr2[11];
	arr2[0] = id;
	memcpy(arr2 + 1, arr, sizeof(arr));
	Message *message = new Message(id , arr, sizeof(arr));

	BOOST_CHECK_EQUAL(message->get_id(), id);
	BOOST_CHECK_EQUAL(message->get_msg_size(), sizeof(arr));
	BOOST_CHECK_EQUAL(message->get_code(), arr2);

}