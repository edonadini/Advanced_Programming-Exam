import datetime


class PostcardList():
        def __init__(self):
                self._file = ''
                self._postcards = []
                self._date = {}
                self._from = {}
                self._to = {}

        def readFile(self, inputfile):
            self._file = inputfile
            with open(self._file, 'r') as f:
                self._postcards = f.readlines()
            self.parsePostcards()
            
        def updateLists(self, new_inputfile):
            self._file = new_inputfile
            with open(self._file, 'r') as f:
                for line in f:
                    self._postcards.append(line)
            self.parsePostcards()
            
        def writeFile(self, outputfile):
            with open(outputfile,'w') as f:
                for elem in self._postcards:
                    f.write(elem)

        def updateFile(outputfile):
            with open (outputfile, 'a') as f:
                for elem in self._postcards:
                    f.write (elem)

        def parsePostcards(self):
            
            # to avoid duplications in the dictionaries
            self._date = {}
            self._from = {}
            self._to = {}
            
            for i, pcard in enumerate(self._postcards):
                pcard = pcard.split(";")
                date_key = datetime.datetime.strptime(pcard[0].split(":")[1],"%Y-%m-%d").date()
                from_key = pcard[1].split(":")[1]
                to_key = pcard[2].split(":")[1]
                
                if date_key not in self._date:
                    self._date[date_key] = [i]      
                else:
                    self._date[date_key].append(i)

                if from_key not in self._from:
                    self._from[from_key] = [i]
                else:
                    self._from[from_key].append(i)
                
                if to_key not in self._to:
                    self._to[to_key] = [i]
                else:
                    self._to[to_key].append(i)
                    
                
        def getNumberOfPostcards(self):
            return len(self._postcards)
    
        def getPostcardsByDateRange(self,date_range):
            # date_range is a tuple
            daterange_list = []
            for date in self._date:
                if date_range[0].date() <= date <= date_range[1].date():
                    for i in self._date[date]:
                        daterange_list.append(self._postcards[i])
            return daterange_list
                
        def getPostcardsBySender(self, sender):
                sender_list = []
                if sender in self._from:
                    for i in self._from[sender]:
                        sender_list.append(self._postcards[i])
                return sender_list

        def getPostcardsByReceiver(self, receiver):
                receiver_list = []
                if receiver in self._to:
                    for i in self._to[receiver]:
                        receiver_list.append(self._postcards[i])
                return receiver_list
