Python project

As for Pyhton module, we are asked to write a class PostcardList.

This class should read/write Postcard messages from/to a properly formatted file, in which each record (i.e. file’s line) is a Postcard; in particular, we are provided with ten of those files (“exam_postcard_listX.txt”). The Postcard format is "date:$(DATE); from:$(SENDER); to:$(RECEIVER);" (e.g., "date:2009-12-24; from:Daisy; to:Donald;"). The class implementation must satisfy the proper tests (defined using unittest) as specified in the assignment. 


This folder contains all the files related to Python assignment:
- A commented .py file named “exam_solution.py”, in which we implemented the PostcardsList class, including the relevant tests imported from the module unittest;
- An “exam_solution_plain.py” file, same as the previous one but without comments.
- The “exam_requests.py” file, containing all the specifications about the assignment;
- All the ten properly formatted files containing the postcards that serve us as input files to build our class;
- A plain text file, named “output.txt”, to write out information;
- Another implementation of the solution, with the same content, but provided in a .ipynb file to be opened with Jupyter Notebook and containing some “naive” step-by-step test that we used to assess the correcteness of our solution.

As for the implementation, the class PostcardList must have at least the following attributes: _file, _postcards,  _date, _from, _to. 
The readFile method is used to read line by line – for the first time – the content of a file and to save it in the list self._postcards; for the subsequent readings, we should use the updateList method, which appends the new lines read from a file to the existing ones already stored in the list. Everytime we read or update, we should also call parsePostcards, which parses the list and sets accordingly the three dictionaries _date, _from and _to.
In particular, the dictionaries are useful to retrieve information about all the postacards respectively sent by a given sender (by means of the function getPostcardsBySender), received by a given receiver (getPostcardsByReceiver), sent within a given date range (getPostcardsByDateRange).
In addition, we can use writeFile to write the content of _postcards into a proper file (in our case, “output.txt”), while with updateFile we can append to the aforementioned file the content of _postcards.
