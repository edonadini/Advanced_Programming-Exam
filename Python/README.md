# Python project

As for Pyhton module, we are asked to write a class PostcardList.

This class should read/write Postcard messages from/to a properly formatted file, in which each record (i.e. file’s line) is a Postcard; in particular, we are provided with ten of those files (**exam_ postcard_listX.txt**). The Postcard format is "date:$(DATE); from:$(SENDER); to:$(RECEIVER);" (e.g., "date:2009-12-24; from:Daisy; to:Donald;"). 

The class implementation must satisfy the proper tests (defined using unittest) as specified in the assignment. 


This folder contains all the files related to Python assignment:
- A .py file named **exam_solution.py**, in which we implemented the PostcardsList class;
- The **exam_requests.py** file, containing all the specifications about the assignment and the relevant tests imported from the module unittest;
- The ten properly formatted files we were provided with, containing the postcards.

As for the implementation, the class PostcardList has the following attributes: _file, _postcards,  _date, _from, _to. 
The **readFile** method is used to read line by line – for the first time – the content of a file and to store it in the list self._postcards; for the subsequent readings, we should use the **updateList** method, which appends the new lines read from a file to the existing ones already stored in the list. Everytime we read or update, we should also call **parsePostcards**, which parses the list and sets accordingly the three dictionaries _date, _from and _to.
In particular, the dictionaries are useful to retrieve information about all the postcards respectively sent by a given sender (by means of the function **getPostcardsBySender**), received by a given receiver (**getPostcardsByReceiver**), sent within a given date range (**getPostcardsByDateRange**).
In addition, we can use **writeFile** to write the content of _postcards list into a file, while with **updateFile** we can append to the file the content of _postcards.

We verified that our class PostcardList satisfies the tests.


