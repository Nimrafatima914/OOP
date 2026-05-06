#include <iostream>
#include <string>
using namespace std;
class LibraryResource { // base class 
protected:
    int resourceID;
    string title;
    string author;
    bool isAvailable;


public:
    LibraryResource(int id = 0, string t = "", string a = "") {
        this->resourceID = id;   // this pointer used to differentiate between member variables and parameters
        this->title = t;
        this->author = a;
        this->isAvailable = true; //it is available when created
    }

    int getID() { return resourceID; }
    bool getStatus() { return isAvailable; }

    void setAvailable(bool status) {
        this->isAvailable = status; // this pointer used in a function , refers to current object 
    }

    void displayDetails() {
        cout << "ID: " << resourceID
            << " | Title: " << title
            << " | Author: " << author
            << " | Status: ";
        if (isAvailable) {
            cout << "Available";
        }
        else {
            cout << "Borrowed";
        }
    }
    
    int calculateLateFee(int daysLate) { // function hiding
        return 0;
    }
};

class Book : public LibraryResource { // derived class
    string ISBN;
    int pageCount;

public:
    Book(int id = 0, string t = "", string a = "", string isbn = "", int pages = 0) // constructor initilizer list
        : LibraryResource(id, t, a) {
        ISBN = isbn;
        pageCount = pages;
    }

    int calculateLateFee(int daysLate) { // Function Hiding
        return daysLate * 5;
    }
};

class Magazine : public LibraryResource { // derived class
    int issueNumber;

public:
    Magazine(int id = 0, string t = "", string a = "", int issue = 0) 
        : LibraryResource(id, t, a) {
        issueNumber = issue;
    }

    int calculateLateFee(int daysLate) {
        return daysLate * 3;
    }
};

class DVD : public LibraryResource { // derived class
    int duration;

public:
    DVD(int id = 0, string t = "", string a = "", int dur = 0)
        : LibraryResource(id, t, a) {
        duration = dur;
    }

    int calculateLateFee(int daysLate) {
        return daysLate * 10;
    }
};

class LibraryMember { // seperate class for members
private:
    int memberID;
    string name;
    LibraryResource borrowedItems[5]; // composition
    int itemCount;

public:
    LibraryMember(int id = -1, string n = "") {
        this->memberID = id;
        this->name = n;
        this->itemCount = 0;
    }

    int getMemberID() { return memberID; }

    LibraryMember& borrowResource(LibraryResource res) { // borrow function
        if (itemCount < 5) {
            if (res.getStatus()) {
                res.setAvailable(false);
                borrowedItems[itemCount++] = res;
                cout << "Resource borrowed successfully.\n";
            }
            else {
                cout << "Resource already borrowed.\n";
            }
        }
        else {
            cout << "Borrow limit reached!\n";
        }

        return *this; // this pointer to return current object
    }

  
    void returnResource(int id) {
        for (int i = 0; i < itemCount; i++) {
            if (borrowedItems[i].getID() == id) {
                for (int j = i; j < itemCount - 1; j++) {
                    borrowedItems[j] = borrowedItems[j + 1];
                }
                itemCount--;
                cout << "Resource returned successfully.\n";
                return;
            }
        }
        cout << "Resource not found.\n";
    }

    void displayBorrowedItems() {
        if (itemCount == 0) {
            cout << "No items borrowed.\n";
            return;
        }

        for (int i = 0; i < itemCount; i++) {
            borrowedItems[i].displayDetails();
            cout << endl;
        }
    }

    int calculateTotalLateFee(int days) {
        int total = 0;
        for (int i = 0; i < itemCount; i++) {
            total += borrowedItems[i].calculateLateFee(days);
        }
        return total;
    }

    // Friend function
    friend void adminView(LibraryResource[], int, LibraryMember[], int);
};


void adminView(LibraryResource res[], int rCount, LibraryMember mem[], int mCount) { // friend function to access private members of libraryMember class
    cout << "\n--- ADMIN VIEW ---\n";

    cout << "\nAll Resources:\n";
    for (int i = 0; i < rCount; i++) {
        res[i].displayDetails();
        cout << endl;
    }

    cout << "\nAll Members:\n";
    for (int i = 0; i < mCount; i++) {
        cout << "Member ID: " << mem[i].memberID
            << " | Name: " << mem[i].name << endl;
        mem[i].displayBorrowedItems();
    }
}

int main() {
    LibraryResource resources[50];
    LibraryMember members[20];

    int rCount = 0, mCount = 0;
    int choice;

    while (true) {
        cout << "\n1.Add Resource\n2.Add Member\n3.Borrow\n4.Return\n5.Show Resources\n6.Show Member\n7.Calculate Fee\n8.Admin View\n9.Exit\nChoice: ";
        cin >> choice;

        if (choice == 9) break;

       
        if (choice == 1) { // add resource
            int id, type;
            string title, author;

            cout << "Enter type (1.Book 2.Magazine 3.DVD): ";
            cin >> type;

            cout << "Enter ID: ";
            cin >> id;

            cin.ignore();

            cout << "Enter Title: ";
            getline(cin, title);

            cout << "Enter Author: ";
            getline(cin, author);

            if (type == 1)
                resources[rCount++] = Book(id, title, author, "ISBN", 100);
            else if (type == 2)
                resources[rCount++] = Magazine(id, title, author, 1);
            else if (type == 3)
                resources[rCount++] = DVD(id, title, author, 120);
            else
                cout << "Invalid type!\n";

            cout << "Resource added successfully!\n";
        }
        else if (choice == 2) { // add member
            int id;
            string name;

            cout << "Enter Member ID: ";
            cin >> id;

            cin.ignore();

            cout << "Enter Name: ";
            getline(cin, name);

            members[mCount++] = LibraryMember(id, name);
            cout << "Member added successfully!\n";
        }
        else if (choice == 3) { // borrow resource
            int memberid, resourceid;

            cout << "Enter Member ID: ";
            cin >> memberid;

            cout << "Enter Resource ID: ";
            cin >> resourceid;

            bool mFound = false, rFound = false;

            for (int i = 0; i < mCount; i++) {
                if (members[i].getMemberID() == memberid) {
                    mFound = true;

                    for (int j = 0; j < rCount; j++) {
                        if (resources[j].getID() == resourceid) {
                            rFound = true;
                            members[i].borrowResource(resources[j]);
                            break;
                        }
                    }
                }
            }

            if (!mFound) cout << "Member not found!\n";
            else if (!rFound) cout << "Resource not found!\n";
        }

  
        else if (choice == 4) { // return resource
            int mid, rid;

            cout << "Enter Member ID: ";
            cin >> mid;

            cout << "Enter Resource ID: ";
            cin >> rid;

            bool found = false;

            for (int i = 0; i < mCount; i++) {
                if (members[i].getMemberID() == mid) {
                    found = true;
                    members[i].returnResource(rid);
                }
            }

            if (!found) cout << "Member not found!\n";
        }

        else if (choice == 5) { // show resources
            cout << "\n--- All Resources ---\n";
            for (int i = 0; i < rCount; i++) {
                resources[i].displayDetails();
                cout << endl;
            }
        }

        else if (choice == 6) { // show member
            int id;

            cout << "Enter Member ID: ";
            cin >> id;

            bool found = false;

            for (int i = 0; i < mCount; i++) {
                if (members[i].getMemberID() == id) {
                    found = true;
                    members[i].displayBorrowedItems();
                }
            }

            if (!found) cout << "Member not found!\n";
        }

        else if (choice == 7) { // calculate fee
            int id, days;

            cout << "Enter Member ID: ";
            cin >> id;

            cout << "Enter days late: ";
            cin >> days;

            bool found = false;

            for (int i = 0; i < mCount; i++) {
                if (members[i].getMemberID() == id) {
                    found = true;
                    cout << "Total Fee: "
                        << members[i].calculateTotalLateFee(days)
                        << " Rs\n";
                }
            }

            if (!found) cout << "Member not found!\n";
        }

       
        else if (choice == 8) { // admin view
            adminView(resources, rCount, members, mCount);
        }
    }

    return 0;
}


