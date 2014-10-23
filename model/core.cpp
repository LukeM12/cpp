#include <boost/thread/thread.hpp>  
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>  
#include <algorithm>
#include <vector>
#include <boost/cstdint.hpp>
const int max_sum_item = 1000000000;
const int threads_to_use = 4;
std::vector<uint64_t *> part_sums;


static void kick(boost::uint64_t *total)
{
  boost::uint64_t localTotal = 0;
 
  for (int i = 0; i < max_sum_item; i++)
    localTotal += i;
 
  *total = localTotal;
}

void do_partial_sum(uint64_t *final, int start_val, int sums_to_do)
{
    uint64_t sub_result = 0;
 
    for (int i = start_val; i < start_val + sums_to_do; i++)
        sub_result += i;
 
    *final = sub_result;

}

int main()
{
	//Clear out the Array of values to be shared
	part_sums.clear();
	for (int i = 0; i < threads_to_use; i++)
    	part_sums.push_back(new uint64_t(0));

    //A Vectors of threads to share the work
	std::vector<boost::thread *> t;
	int sums_per_thread = max_sum_item / threads_to_use;


	boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();

	//boost::thread worker(kick, &result);

	for (int start_val = 0, i = 0; start_val < max_sum_item; start_val += sums_per_thread, i++)
	{
	  t.push_back(new boost::thread(do_partial_sum, part_sums[i], start_val, sums_per_thread));
	}
	for (int i = 0; i < threads_to_use; i++)
    	t[i]->join();
 	boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
   	boost::posix_time::time_duration timeTaken = end - start;
   	uint64_t result = 0;

   	/* Clear memory */
   	for (int i = 0; i < threads_to_use; i++)
{
  delete t[i];
  delete part_sums[i];
}
 
std::for_each(part_sums.begin(), part_sums.end(),
    [&result] (uint64_t *subtotal) { result += *subtotal; });
    std::cout << result ;
	std::cout <<'\n';
	std::cout << timeTaken << std::endl;
}


/*int main()
{
 	//boost::uint64_t result = 0;
 	  uint64_t result;
 	boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
    boost::thread worker(do_sum, &result);
    worker.join();


    for (int i = 0; i < max_sum_item; i++)
    	result += i;
 	boost::this_thread::sleep(boost::posix_time::milliseconds(500));
 	boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
   	boost::posix_time::time_duration timeTaken = end - start;
    std::cout << result ;
	std::cout <<'\n';
	std::cout << timeTaken << std::endl;
}

int main(int argc, char* argv[])  
{  
    boost::thread thrd(&hello);  
    thrd.join();  
    return 0;  
}  */