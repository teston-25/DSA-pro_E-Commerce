#include<iostream>
#include<sqlite3.h>
#include <stack>
#include <queue>

using namespace std;

string email;
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        email = argv[3];
    }
    return 0;
}

//products database
void Products()
{
    sqlite3 *db;
    int rc = sqlite3_open("items.db", &db);
    char *errMsg = nullptr;
    const char *sql;
    cout << "############################################" << endl;
    cout << "#                  PRODUCTS                #" << endl;
    cout << "############################################" << endl;
    cout << "Id"
         << " | "
         << "Name"
         << " | "
         << "Price"
         << " | "
         << "Description"
         << endl;
    cout <<"############################################" << endl;

    sql = "SELECT * FROM product_info;";
    rc = sqlite3_exec(
        db, sql, [](void *data, int argc, char **argv, char ** /*colNames*/) -> int
        {
        for (int i = 0; i < argc; i++) {
            cout << argv[i] << " | ";
        }
        cout << endl;
        return 0; },
        nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

//ongoing products
void orders_ongoing()
{
    sqlite3 *db;
    int rc = sqlite3_open("items.db", &db);
    char *errMsg = nullptr;
    cout<< "############################################" << endl;
    cout<< "#               ONGOING ORDERS             #" << endl;
    cout<< "############################################" << endl;
    cout<<"order_id"
        << " | "
        << "product_name"
        << " | "
        << "Date"
        << endl;
    cout << "############################################" << endl;
    const char *sql = "SELECT * FROM orders;";
    rc = sqlite3_exec(
        db, sql, [](void *data, int argc, char **argv, char ** /*colNames*/) -> int
        {
        for (int i = 0; i < argc; i++) {
            cout << argv[i] << " | ";
        }
        cout << endl;
        return 0; },
        nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

//signin user authentication
void usersignin(){
    sqlite3 *db;
    int rc = sqlite3_open("items.db", &db);
    if (rc)
    {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
    }
    string email,password;
    cout << "############################################" << endl;
    cout << "#                 SIGN IN                  #" << endl;
    cout << "############################################" << endl;
    cout << "Enter Your Email: ";
    cin >> email;
    cout << "Enter Your Password: ";
    cin >> password;
    cout<<endl;

    string sql = "SELECT * FROM users WHERE email = ? AND password = ? AND EXISTS (SELECT email,password FROM users WHERE email = ? AND password = ?)";
    char *errMsg = nullptr;

    rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error executing SQLite query: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
    }
    else{
      cout<<"you have logged in successfully"<<endl;
    }

    if (email.empty())
    {
        std::cout << "Invalid email or password." << std::endl;
        sqlite3_close(db);
    }

    sqlite3_close(db);
};

//registering user
void registeruser(){
 string email,username,password;
  sqlite3 *db;
  int rc = sqlite3_open("items.db", &db);
  if (rc != SQLITE_OK) {
    cerr << "Failed to open database file: " << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return;
  }

  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, "INSERT INTO users(username, email, password) VALUES (?, ?, ?);", -1, &stmt, NULL);
   const char *sql;
    cout << "############################################" << endl;
    cout << "#                 SIGN UP                  #" << endl;
    cout << "############################################" << endl;
    cout<<"Enter user name:";
    cin>>username;
    cout<<"Enter Email:";
    cin>>email;
    cout<<"Enter password:";
    cin>>password;
    cout << "############################################" << endl;
  if (rc != SQLITE_OK) {
    cerr << "Failed to prepare SQL query: " << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return;
  }
  sqlite3_bind_text(stmt, 1, username.c_str(), username.length(), SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, email.c_str(), email.length(), SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, password.c_str(), password.length(), SQLITE_TRANSIENT);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    cerr << "Failed to execute SQL query: " << sqlite3_errmsg(db) << endl;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
  }
  else
    {
        cout << "User registered Successfully" << endl;
        usersignin();
    }
};
//Adding new product database
void registerProduct( /*string& name, double price, string& description, string& user*/) {
    string name,description,user;
    int price; 
  sqlite3 *db;
  int rc = sqlite3_open("items.db", &db);
  if (rc != SQLITE_OK) {
    cerr << "Failed to open database file: " << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return;
  }

  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, "INSERT INTO product_info(name, price, description) VALUES (?, ?, ?);", -1, &stmt, NULL);
   const char *sql;
    cout << "############################################" << endl;
    cout << "#             ADD NEW PRODUCT              #" << endl;
    cout << "############################################" << endl;
    cout<<"Enter product name:";
    cin>>name;
    cout<<"Enter product price:";
    cin>>price;
    cout<<"Enter product description:";
    cin>>description;
    cout << "############################################" << endl;
  if (rc != SQLITE_OK) {
    cerr << "Failed to prepare SQL query: " << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return;
  }
  sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), SQLITE_TRANSIENT);
  sqlite3_bind_double(stmt, 2, price);
  sqlite3_bind_text(stmt, 3, description.c_str(), description.length(), SQLITE_TRANSIENT);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    cerr << "Failed to execute SQL query: " << sqlite3_errmsg(db) << endl;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
  }
  else
    {
        cout << "Product registered Successfully" << endl;
    }
};

//ordering products
void order_product(){
string name,id;
int price;
Products();
cout<<endl;
cout<< "############################################" << endl;
cout<< "#              TO ORDER PRODUCT            #" << endl;
cout<< "############################################" << endl;
string prod_id;
cout << "Enter product id from above table:";
cin >> prod_id;
cout << "############################################" << endl;

sqlite3 *db;
int rc = sqlite3_open("items.db", &db);
if (rc != SQLITE_OK) {
  cerr << "Failed to open database file: " << sqlite3_errmsg(db) << endl;
  sqlite3_close(db);
  return;
}
 const char *sql = "INSERT INTO orders ( product_name, order_date) VALUES ((SELECT name FROM product_info WHERE id = ?)  ,datetime('now'))";

    sqlite3_stmt *stmt;    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt,NULL);
    if (rc != SQLITE_OK)
    {
        cout << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        exit(0);
    }
 sqlite3_bind_text(stmt, 1, prod_id.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        cout << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        cout << "Error executing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
    else
    {
      cout << "Ordered Product Successfully" << endl;

    const char *sql ="DELETE FROM product_info WHERE id =(SELECT id FROM product_info WHERE name ='?')";
        sqlite3_stmt *stmt;    

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt,NULL);

    if (rc != SQLITE_OK)
    {
        cout << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        exit(0);
    }
sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

      cout<<"product stack has updated!"<<endl;
    }
sqlite3_finalize(stmt);
}

//introduction background
void intro(){
  int input;
    cout << "############################################" << endl;
    cout << "    E-COMMERCE INVENTORY MANAGMENT SYSTEM                    " << endl;
    cout << "############################################" << endl;
    cout<<"1.TO WATCH PRODUCTS"<<endl;
    cout<<"2.ADD NEW PRODUCTS"<<endl;
    cout<<"3.TO ORDER PRODUCT"<<endl;
    cout<<"4.ONGOING ORDERS"<<endl;
    cout<<"5.USER SIGNUP"<<endl;
    cout<<"6.SIGNIN USERS"<<endl;
    cout<<"7.EXIT"<<endl;
    cout<< "############################################" << endl;
    cout<<"Enter the service number you would like to get:";
    cin>>input;
  if(input==1){
    Products();
      return intro();
  }
  else if(input==2){
        registerProduct( );
      return  intro();
  }
  else if(input==3){
        order_product();
      return intro();
  }
  else if(input==4){
        orders_ongoing();
      return intro();
  }
  else if(input==5){
        registeruser();
      return intro();
  }
  else if(input==6){
        usersignin();
      return intro();
  }
  else if(input==7){
    exit;
  }
}

int main(int argc, char const *argv[])
{
  intro();
    return 0;
}
