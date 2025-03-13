#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

class Book{
    private:
        string title;
        string author;
        string ISBN;
        bool availability;
    public:
        Book(string title, string author, string ISBN): title(title), author(author), ISBN(ISBN), availability(true){}

        string getTitle() const {return title;}
        string getAuthor() const { return author;}
        string getISBN() const { return ISBN;}
        bool isAvailable () const { return availability;}

        void borrowBook(){availability = false;}
        void returnBook() {availability = true;}

        void displayBook() const {
            cout << ">> Title: " << title << "\n"
                << ">> Author: " << author << "\n"
                << ">> ISBN: " << ISBN << "\n"
                << ">> Available? " << (availability ? "True" : "False") << "\n";
        }
};

class User{
    protected:
        string name;
        int userID; //can be generated automaticaly
        static int nextID;
        vector<Book*> borrowedBooks;//dynamic list of borrowed books
    public:
        User(string name) : name(name) {
            userID = nextID++;
        }

        int getUserID() const { return userID; }
        string getName() const { return name; }
        //void setName(string name){this.name = name;}

        void borrowBook(Book* book) {
            if (book->isAvailable()) {
                borrowedBooks.push_back(book);
                book->borrowBook();
                cout << name << " borrowed: " << book->getTitle() << endl;
            } else {
                cout << "Book is not available for borrowing.\n";
            }
        }

        void returnBook(string title) {
            for (size_t i = 0; i < borrowedBooks.size(); i++) {
                if (borrowedBooks[i]->getTitle() == title) {
                    borrowedBooks[i]->returnBook();
                    cout << name << " returned: " << borrowedBooks[i]->getTitle() << endl;
                    borrowedBooks.erase(borrowedBooks.begin() + i);
                    return;
                }
            }
            cout << "No book with title \"" << title << "\" was borrowed by " << name << endl;
        }

        void displayUser() const {
            cout << ">> User ID: " << userID << "\n"
                 << ">> Name: " << name << "\n"
                 << ">> Borrowed Books: ";
            if (borrowedBooks.empty()) {
                cout << "None\n";
            } else {
                cout << "\n";
                for (const auto& book : borrowedBooks) {
                    cout << "  - " << book->getTitle() << "\n";
                }
            }
        }
};
int User::nextID = 1;


class Student: public User{
    public:
    Student(string name) : User(name) {}

    void displayUser() const {
        cout << "[Student]\n";
        User::displayUser();
    }
};

class Faculty: public User{
    public:
    Faculty(string name) : User(name) {}

    void displayUser() const {
        cout << "[Faculty]\n";
        User::displayUser();
    }
};

//should be singleton
class Library{ //manages books, users, and transactions
    private:
        vector<Book> books;
        vector<User> users;
        static Library* instance;

        Library() {} // Private contruct library
    public:
        vector<Book>& getBooks() { return books; } 
        vector<User>& getUsers() { return users; } 
        static Library* getInstance() {
            if (instance == nullptr) {
                instance = new Library();
            }
            return instance;
        }

        void addBook(string title, string author, string ISBN) {
            books.push_back(Book(title, author, ISBN));
            cout << "Book Added: " << title << endl;
        }

        void listBooks() {
            if (books.empty()) {
                cout << "No books in the library.\n";
                return;
            }
            cout << "Library Books:\n";
            for (const auto& book : books) {
                book.displayBook();
                cout << "-------------------\n";
            }
        }

        void addUser(int type, string name) {
            if (type == 1) {
                users.push_back(Student(name));
            } else if (type == 2) {
                users.push_back(Faculty(name));
            } else {
                cout << "Invalid user type!\n";
                return;
            }
            cout << "User Added: " << name << " (ID: " << users.back().getUserID() << ")\n";
        }

        void listUsers() {
            if (users.empty()) {
                cout << "No users registered.\n";
                return;
            }
            cout << "Registered Users:\n";
            for (const auto& user : users) {
                user.displayUser();
                cout << "-------------------\n";
            }
        }

        Book* findBook(string title) {
            for (auto& book : books) {
                if (book.getTitle() == title) {
                    return &book;
                }
            }
            return nullptr;
        }

        User* findUserByID(int userID) {
            for (auto& user : users) {
                if (user.getUserID() == userID) {
                    return &user;
                }
            }
            return nullptr;
        }

        void borrowBook(int userID, string title) {
            User* user = findUserByID(userID);
            Book* book = findBook(title);
    
            if (user && book) {
                user->borrowBook(book);
            } else {
                cout << "Invalid User ID or Book Title!\n";
            }
        }
    
        void returnBook(int userID, string title) {
            User* user = findUserByID(userID);
            if (user) {
                user->returnBook(title);
            } else {
                cout << "User ID not found.\n";
            }
        }
};
Library* Library::instance = nullptr;

int main(){
    Library* library = Library::getInstance();
    int choice, secondChoice;
    while (true) {
        cout << "\nWelcome to the Library:\n"
             << "1. Manage Books\n"
             << "2. Manage Users\n"
             << "3. Manage Transactions\n"
             << "4. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: //Manage Books
                while (true) {
                    cout << "\n> Manage Books:\n"
                         << "1. Add a Book\n"
                         << "2. Edit a Book\n"
                         << "3. Remove a Book\n"
                         << "4. Go Back\n"
                         << "Enter your choice: ";
                    cin >> secondChoice;

                    switch (secondChoice) {
                        case 1: {
                            string title, author, ISBN;
                            cout << "Enter Book Title: ";
                            cin.ignore();
                            getline(cin, title);
                            cout << "Enter Author: ";
                            getline(cin, author);
                            cout << "Enter ISBN: ";
                            getline(cin, ISBN);
                            library->addBook(title, author, ISBN);
                            break;
                        }
                        case 2: {
                            string title;
                            cout << "Enter Book Title to Edit: ";
                            cin.ignore();
                            getline(cin, title);
                            Book* book = library->findBook(title);
                            if (book) {
                                /*string title, author, ISBN;
                                cout << "Enter New Title: ";
                                getline(cin, title);
                                book->setAuthor(title);
                                cout << "Enter New Author: ";
                                getline(cin, author);
                                book->setAuthor(author);
                                cout << "Enter New ISBN: ";
                                getline(cin, ISBN);
                                book->setAuthor(ISBN);
                                cout << "Book Updated Successfully!\n";*/
                            } else {
                                cout << "Book Not Found!\n";
                            }
                            break;
                        }
                        case 3: {
                            string title;
                            cout << "Enter Book Title to Remove: ";
                            cin.ignore();
                            getline(cin, title);
                            vector<Book>& books = library->getBooks();
                            for (size_t i = 0; i < books.size(); i++) {
                                if (books[i].getTitle() == title) {
                                    books.erase(books.begin() + i);
                                    cout << "Book Removed Successfully!\n";
                                    break;
                                }
                            }
                            break;
                        }
                        case 4:
                            goto exit_books;
                        default:
                            cout << "Invalid choice.\n";
                    }
                }
                exit_books:
                break;

            case 2: // Manage Users
                while (true) {
                    cout << "\n> Manage Users:\n"
                         << "1. Add a User\n"
                         << "2. Edit a User\n"
                         << "3. Remove a User\n"
                         << "4. Go Back\n"
                         << "Enter your choice: ";
                    cin >> secondChoice;

                    switch (secondChoice) {
                        case 1: {
                            int type;
                            string name;
                            cout << "Enter 1 for Student, 2 for Faculty: ";
                            cin >> type;
                            cout << "Enter Name: ";
                            cin.ignore();
                            getline(cin, name);
                            library->addUser(type, name);
                            break;
                        }
                        case 2: {
                            int id;
                            cout << "Enter User ID to Edit: ";
                            cin >> id;
                            User* user = library->findUserByID(id);
                            if (user) {
                                /*string name;
                                cout << "Enter New Name: ";
                                cin.ignore();
                                getline(cin, name);
                                User->setName(name);
                                cout << "User Updated Successfully!\n";*/
                            } else {
                                cout << "User Not Found!\n";
                            }
                            break;
                        }
                        case 3: {
                            int id;
                            cout << "Enter User ID to Remove: ";
                            cin >> id;
                            vector<User>& users = library->getUsers();
                            for (size_t i = 0; i < users.size(); i++) {
                                if (users[i].getUserID() == id) {
                                    users.erase(users.begin() + i);
                                    cout << "User Removed Successfully!\n";
                                    break;
                                }
                            }
                            break;
                        }
                        case 4:
                            goto exit_users;
                        default:
                            cout << "Invalid choice.\n";
                    }
                }
                exit_users:
                break;

            case 3: // Borrow/Return Books
                while (true) {
                    cout << "\n> Borrow/Return Books:\n"
                         << "1. Check Out a Book\n"
                         << "2. Check In a Book\n"
                         << "3. List All Books\n"
                         << "4. List All Users\n"
                         << "5. Go Back\n"
                         << "Enter your choice: ";
                    cin >> secondChoice;

                    switch (secondChoice) {
                        case 1: {
                            int userID;
                            string title;
                            cout << "Enter User ID: ";
                            cin >> userID;
                            cout << "Enter Book Title: ";
                            cin.ignore();
                            getline(cin, title);
                            library->borrowBook(userID, title);
                            break;
                        }
                        case 2: {
                            int userID;
                            string title;
                            cout << "Enter User ID: ";
                            cin >> userID;
                            cout << "Enter Book Title: ";
                            cin.ignore();
                            getline(cin, title);
                            library->returnBook(userID, title);
                            break;
                        }
                        case 3:
                            library->listBooks();
                            break;
                        case 4:
                            library->listUsers();
                            break;
                        case 5:
                            goto exit_borrow;
                        default:
                            cout << "Invalid choice.\n";
                    }
                }
                exit_borrow:
                break;

            case 4:
                cout << "Exiting Library System.\n";
                return 0;

            default:
                cout << "Invalid choice.\n";
        }
    }

    
    return 0;
}