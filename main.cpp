
#include "components/books.h"
#include "lib/json/json.hpp"
#include <conio.h>
#include <fstream>
#include <iostream>
#include <vector>

using json = nlohmann::json;
using namespace std;

bool validateUserDetails(string username, string password);
void ShowAllBooks();
void AddANewBook();
void DeleteBook();
void FindEmptySlot();
void UpdateBookDetails();

int main(int argc, char const *argv[]) {
    cout << endl << endl;
    cout << "LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << "-------------------------" << endl;
    cout << "Login to your administrator account" << endl;

    string username, password;
    char ch;

    cout << "Enter username : ";
    cin >> username;

    cout << "Enter Password : ";
    ch = _getch();
    while (ch != 13) {
        // charcter 13 is enter (if enter is pressed)
        password.push_back(ch);
        cout << '*';
        ch = _getch();
    }
    cout << endl;

    if (!validateUserDetails(username, password)) {
        cout << "Try Again!!";
        exit(0);
    }

    // print choices
    int choice = 99;
    while (choice = 99) {
        cout << endl;
        cout << "Please select your choice : " << endl;
        cout << "1. To show all books" << endl;
        cout << "2. Add a new Book" << endl;
        cout << "3. Delete a book" << endl;
        cout << "4. Find a empty slot" << endl;
        cout << "5. Update the Book Details" << endl;
        cout << "99 to continue" << endl;
        cout << "00 to exit" << endl;

        int userInput;
        cin >> userInput;

        switch (userInput) {
        case 1:
            ShowAllBooks();
            break;
        case 2:
            AddANewBook();
            break;
        case 3:
            DeleteBook();
            break;
        case 4:
            FindEmptySlot();
            break;
        case 5:
            UpdateBookDetails();
            break;
        case 99:
            continue;
        case 00:
            cout << endl << "Thanks for Using the System" << endl << endl;
            exit(0);
            break;

        default:
            exit(0);
            break;
        }
    }

    return 0;
}

bool validateUserDetails(string username, string password) {
    // function to validate username and password
    if (username == "") {
        cout << "Error:- Username is required..!!" << endl;
        return false;
    }

    if (password == "") {
        cout << "Error:- Password cannot be null..!!" << endl;
        return false;
    }

    ifstream f("./database/users.json");
    json data = json::parse(f);
    f.close();

    // tarversing all the users
    for (int i = 0; i < data["data"].size(); i++) {
        if (data["data"][i]["user_name"] == username) {
            // check for password
            if (data["data"][i]["password"] == password) {
                // correct password
                cout << "Done... Loading" << endl;
                return true;
            }
            else {
                cout << "Oops! Incorrect password" << endl;
                return false;
            }
        }
    }
    cout << "Error:- Invalid credientials" << endl;
    return false;
}

void ShowAllBooks() {
    // to print all the books
    ifstream f("./database/books.json");
    json data = json::parse(f);
    f.close();

    if (data["data"].size() == 0) {
        cout << "No Books Available";
        return;
    }
    for (int i = 0; i < data["data"].size(); i++) {
        cout << endl;
        cout << "Book ID :-  " << data["data"][i]["id"] << endl;
        cout << "Name    :-  " << data["data"][i]["name"] << endl;
        cout << "Price   :-  " << data["data"][i]["price"] << endl;
        cout << "Author  :-  " << data["data"][i]["author"] << endl;
        cout << endl;
    }
}

void AddANewBook() {
    ifstream f1("./database/books.json");

    json data = json::parse(f1);
    // cout << data;
    f1.close();

    if (data["data"].size() >= 20) {
        cout << "Space is full!! Unable to add new Book" << endl;
        return;
    }

    string name, author;
    int price;
    cout << "Enter The New Book Name :- ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter The Book Author Name :- ";
    getline(cin, author);
    cout << "Enter The Book Price :- ";
    cin >> price;

    if (name == "" || author == "") {
        cout << "Error:- Name, Author, Price cannot be empty. Try Again"
             << endl;
        return;
    }
    int tempId = data["data"].size() + 1;
    json newData = {
        {"id", tempId}, {"name", name}, {"author", author}, {"price", price}};
    data["data"].push_back(newData);

    ofstream f2("./database/books.json");
    f2 << data;
    f2.close();
    cout << "Book Added Succesfully" << endl;
}

void DeleteBook() {
    int BookID;
    cout << endl << "Enter the Book ID :- ";
    cin >> BookID;

    ifstream f1("./database/books.json");
    json data = json::parse(f1);
    f1.close();
    if (data["data"].size() < BookID) {
        cout << "Invalid BookID Try Again" << endl;
        return;
    }
    char userConfirmation;
    cout << "Book Name to be deleted :- " << data["data"][BookID - 1]["name"]
         << endl;
    cout << "Enter y to continue and any other key to cancel :-";
    cin >> userConfirmation;
    if (userConfirmation != 'y') {
        cout << "Cancelled !!" << endl;
        return;
    }
    data["data"][BookID - 1]["id"] = NULL;
    data["data"][BookID - 1]["name"] = NULL;
    data["data"][BookID - 1]["author"] = NULL;
    data["data"][BookID - 1]["price"] = NULL;

    ofstream f2("./database/books.json");
    f2 << data;
    f2.close();
    cout << "Book Deleted Sucessfully" << endl;
}

void FindEmptySlot() {
    vector<int> slotsList;
    ifstream f1("./database/books.json");
    json data = json::parse(f1);
    f1.close();

    for (int i = 0; i < data["data"].size(); i++) {
        if (data["data"][i]["id"] == NULL) {
            // Slot Found push into array
            slotsList.push_back(i + 1);
        }
    }
    if (slotsList.empty()) {
        cout << "No Empty Slots Found";
        return;
    }

    cout << "Empty Slots are :- ";
    for (auto it = slotsList.begin(); it != slotsList.end(); it++)
        cout << *it << " ,";
    cout << endl;
}

void UpdateBookDetails() {
    int BookID;
    cout << "Enter the Book ID :- ";
    cin >> BookID;

    ifstream f1("./database/books.json");
    json data = json::parse(f1);
    f1.close();

    if (data["data"].size() < BookID) {
        cout << "Invalid Book ID" << endl;
        return;
    }
    cout << endl << "Book Details" << endl;
    cout << "ID :- " << data["data"][BookID - 1]["id"] << endl;
    cout << "Name :- " << data["data"][BookID - 1]["name"] << endl;
    cout << "Author :- " << data["data"][BookID - 1]["author"] << endl;
    cout << "Price :- " << data["data"][BookID - 1]["price"] << endl;

    string name, author;
    int price;
    cout << "New Book Name :- ";
    cin.ignore();
    getline(cin, name);
    cout << "Author Name :- ";
    getline(cin, author);
    cout << "Price :- ";
    cin >> price;

    // Updating the data
    data["data"][BookID - 1]["name"] = name;
    data["data"][BookID - 1]["author"] = author;
    data["data"][BookID - 1]["price"] = price;

    ofstream f2("./database/books.json");
    f2 << data;
    f2.close();
    cout << "Book Updated Sucessfully" << endl;
}