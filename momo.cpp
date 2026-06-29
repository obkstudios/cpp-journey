/* BUILDING MOMO APP
------------WHAT IT DOES--------------
They go to the main menu
They want to do transactions, but if the user is not registered as an account holder,
    they have to choose the option to create an account first before they can make any transactions
Menu:
At the menu

User enters their contact.
They create a mobile money accoutn
 - They set the pin for the password
 - They receive a one time password on their device to verify the contact before being able to set the password
 - Account created successfully

They go to the main menu
1. Transfer Money
2. MomoPay
3. Allow Cash Out
4. My Wallet
5. Exit

-------------Transfer Money-----------------
Enter receipient contact
Press 0 to go back

Confirm receipient contact
Enter Amount
Enter reference
You are sending ... to (receipient contact). Total amount is ... (maybe plus fees) Enter MM PIN or 2 to cancel

-------------MomoPay and Pay Bill----------------
  1. Enter merchant id
  2. back to main menu
  3. Exit

  - confirm merchant id
  - enter amount
  - you are sending ... to ... . Total: ... (maybe plus fees) Enter MM PIN or 2 to cancel

-------------Allow Cash Out--------------------
Are you sure you want to allow cash out?
  - [Y] / [N]
Cash out allowed.
0. Go back
1. exit
Send user back to main menu


-------------My Wallet-------------------
1. Check Balance
2. Allow Cash Out
3. My approvals
4. Change and reset PIN
5. Back to main menu
*/

#include <iostream>
#include <string>
#include <cstdlib>

// This returns the user to the main menu
void returnToMain()
{
  while (true)
  {
    std::cout << "\n0. Exit   1. Back to Main Menu \nChoice: ";
    int choice{};
    std::cin >> choice;

    if (choice == 0)
    {
      std::cout << "Thank you for using GnG. \nExiting...";
      exit(0);
    }
    else if (choice == 1)
    {
      return;
    }
    else
    {
      std::cout << "Invalid entry. ";
    }
  }
}

// Clears the content of the buffer so that new inputs
void clearInput()
{
  std::cin.clear();
  std::cin.ignore(100, '\n');
}

class Account
{
public:
  std::string name;
  std::string contact;

  int momoPin{};
  double balance{};
  bool isRegistered{false};
  bool cashOutAllowed{false};

  bool verifyPin(int enteredPin)
  {
    return enteredPin == momoPin;
  }
};

class CreateAccount
{
public:
  Account create(Account acc)
  {
    std::cout << "============= Create Account ==============\n";
    std::cout << "Enter your phone number: ";
    std::cin >> acc.contact;
    clearInput();

    std::cout << "Enter the name for your account: ";
    std::getline(std::cin, acc.name);

    int pin1{}, pin2{};
    std::cout << "Create a 4-digit GnG PIN: ";
    std::cin >> pin1;
    std::cout << "Confirm the GnG PIN: ";
    std::cin >> pin2;

    if (pin1 != pin2)
    {
      std::cout << "PINs do not match. Try again one more time.\n";
      std::cout << "Create a 4-digit GnG PIN: ";
      std::cin >> pin1;
      std::cout << "Confirm the GnG PIN: ";
      std::cin >> pin2;

      if (pin1 != pin2)
      {
        std::cout << "PINs do not match again. Account creation cancelled. ";
        returnToMain();
        return acc;
      }
    }

    acc.momoPin = pin1;
    acc.balance = 0.0;
    acc.isRegistered = true;

    std::cout << "\nAccount created successfully! Welcome, " << acc.name << ".\n";
    returnToMain();
    return acc;
  }
};

class TransferMoney
{
private:
  const double fee_rate = 0.01;

public:
  Account transfer(Account acc)
  {
    std::cout << "============= Transfer Money ===============\n";

    std::string recipient;
    std::cout << "Enter receipient contact (0 to go back): ";
    std::cin >> recipient;

    if (recipient == "0")
      return acc;

    std::string rp2;
    std::cout << "Confirm receipient contact: ";
    std::cin >> rp2;

    if (recipient != rp2)
    {
      std::cout << "Contacts do not match. Try again.\n";
      std::cout << "Enter receipient contact: ";
      std::cin >> recipient;

      std::cout << "Confirm receipient contact: ";
      std::cin >> rp2;

      if (recipient != rp2)
      {
        std::cout << "Contacts do not match. Transfer Cancelled.\n";
        returnToMain();
        return acc;
      }
    }

    double amount{};
    std::cout << "Enter amount (GHS): ";
    std::cin >> amount;
    if (amount <= 0)
    {
      std::cout << "Invalid amount.\n";
      returnToMain();
      return acc;
    }

    clearInput();
    std::string reference{};
    std::cout << "Enter reference: ";
    std::getline(std::cin, reference);

    double fee = amount * fee_rate;
    double total = amount + fee;

    std::cout << "\nYou are sending GHS " << amount
              << " to " << recipient
              << ".\nFee: GHS " << fee
              << "\nTotal amount: GHS " << total
              << "\n\nEnter MM PIN to confirm or 2 to cancel: ";

    int input{};
    std::cin >> input;
    if (input == 2)
    {
      std::cout << "Transfer cancelled.\n";
      returnToMain();
      return acc;
    }

    if (!acc.verifyPin(input))
    {
      std::cout << "Incorrect PIN. Try one more time or enter 2 to cancel: ";
      std::cin >> input;
      if (input == 2 || !acc.verifyPin(input))
      {
        std::cout << "Transfer cancelled.\n";
        returnToMain();
        return acc;
      }
    }

    if (acc.balance < total)
    {
      std::cout << "Insufficient balance. Transfer failed.\n";
      returnToMain();
      return acc;
    }

    acc.balance -= total;
    std::cout << "Transfer successful! New balance: GHS " << acc.balance << "\n";
    returnToMain();
    return acc;
  }
};

class MomoPay
{
private:
  const double FEE_RATE = 0.005;

public:
  Account pay(Account acc)
  {
    std::cout << "\n========== MomoPay ==========\n";

    while (true)
    {
      std::cout << "1. Enter Merchant ID\n"
                << "2. Back to Main Menu\n"
                << "3. Exit\n"
                << "Choice: ";
      int choice{};
      std::cin >> choice;

      if (choice == 2)
        return acc;
      if (choice == 3)
      {
        std::cout << "Goodbye!\n";
        exit(0);
      }
      if (choice != 1)
      {
        std::cout << "Invalid option.\n";
        continue;
      }

      std::string merchantId{};
      std::cout << "Enter Merchant ID: ";
      std::cin >> merchantId;

      std::string confirmId{};
      std::cout << "Confirm Merchant ID: ";
      std::cin >> confirmId;
      if (merchantId != confirmId)
      {
        std::cout << "Merchant IDs do not match. Try again.\n";
        continue;
      }

      double amount{};
      std::cout << "Enter amount (GHS): ";
      std::cin >> amount;
      if (amount <= 0)
      {
        std::cout << "Invalid amount.\n";
        continue;
      }

      double fee = amount * FEE_RATE;
      double total = amount + fee;

      std::cout << "\nYou are sending GHS " << amount
                << " to Merchant " << merchantId
                << ".\nFee: GHS " << fee
                << "\nTotal: GHS " << total
                << "\n\nEnter MM PIN to confirm or 2 to cancel: ";

      int input{};
      std::cin >> input;
      if (input == 2)
      {
        std::cout << "Payment cancelled.\n";
        returnToMain();
        return acc;
      }

      if (!acc.verifyPin(input))
      {
        std::cout << "Incorrect PIN. One more attempt: ";
        std::cin >> input;
        if (input == 2 || !acc.verifyPin(input))
        {
          std::cout << "Payment cancelled.\n";
          returnToMain();
          return acc;
        }
      }

      if (acc.balance < total)
      {
        std::cout << "Insufficient balance.\n";
        returnToMain();
        return acc;
      }

      acc.balance -= total;
      std::cout << "Payment successful! New balance: GHS " << acc.balance << "\n";
      returnToMain();
      return acc;
    }
  }
};

class AllowCashOut
{
public:
  Account allow(Account acc)
  {
    std::cout << "\n========== Allow Cash Out ==========\n";
    std::cout << "Are you sure you want to allow cash out? [Y/N]: ";
    char choice{};
    std::cin >> choice;

    if (choice == 'Y' || choice == 'y')
    {
      acc.cashOutAllowed = true;
      std::cout << "Cash out allowed.\n";
    }
    else
    {
      std::cout << "Cash out not allowed.\n";
    }
    returnToMain();
    return acc;
  }
};

class MyWallet
{
private:
  AllowCashOut cashOutService{};

  void checkBalance(Account acc)
  {
    std::cout << "\n--- Balance ---\n";
    std::cout << "Account: " << acc.name << "\n";
    std::cout << "Balance: GHS " << acc.balance << "\n";
    returnToMain();
  }

  Account changePin(Account acc)
  {
    std::cout << "\n--- Change / Reset PIN ---\n";
    std::cout << "Enter current PIN: ";
    int current{};
    std::cin >> current;

    if (!acc.verifyPin(current))
    {
      std::cout << "Incorrect PIN. One more attempt: ";
      std::cin >> current;
      if (!acc.verifyPin(current))
      {
        std::cout << "Incorrect PIN. Cancelled.\n";
        returnToMain();
        return acc;
      }
    }

    int newPin{}, confirmPin{};
    std::cout << "Enter new PIN: ";
    std::cin >> newPin;
    std::cout << "Confirm new PIN: ";
    std::cin >> confirmPin;

    if (newPin != confirmPin)
    {
      std::cout << "PINs do not match. Try again.\n";
      std::cout << "Enter new PIN: ";
      std::cin >> newPin;
      std::cout << "Confirm new PIN: ";
      std::cin >> confirmPin;

      if (newPin != confirmPin)
      {
        std::cout << "PINs do not match. No changes made.\n";
        returnToMain();
        return acc;
      }
    }

    acc.momoPin = newPin;
    std::cout << "PIN changed successfully.\n";
    returnToMain();
    return acc;
  }

public:
  Account open(Account acc)
  {
    while (true)
    {
      std::cout << "\n========== My Wallet ==========\n"
                << "1. Check Balance\n"
                << "2. Allow Cash Out\n"
                << "3. Change and Reset PIN\n"
                << "4. Back to Main Menu\n"
                << "Choice: ";
      int choice{};
      std::cin >> choice;

      switch (choice)
      {
      case 1:
        checkBalance(acc);
        break;
      case 2:
        acc = cashOutService.allow(acc);
        break;
      case 3:
        acc = changePin(acc);
        break;
      case 4:
        return acc;
      default:
        std::cout << "Invalid option.\n";
        break;
      }
    }
  }
};

class MainMenu
{
private:
  int optionNum{};

  TransferMoney transferService{};
  MomoPay momoPayService{};
  AllowCashOut cashOutService{};
  MyWallet walletService{};

public:
  void menu(Account &acc)
  {
    while (true)
    {
      std::cout << "\n\n-------------- Welcome to SendeMaMe --------------\n"
                << "Logged in as: " << acc.name << "\n"
                << "Choose from the following options:\n"
                << "1. Transfer Money\n"
                << "2. MomoPay\n"
                << "3. Allow Cash Out\n"
                << "4. My Wallet\n"
                << "5. Exit\n"
                << "Choice: ";

      std::cin >> optionNum;

      switch (optionNum)
      {
      case 1:
        acc = transferService.transfer(acc);
        break;
      case 2:
        acc = momoPayService.pay(acc);
        break;
      case 3:
        acc = cashOutService.allow(acc);
        break;
      case 4:
        acc = walletService.open(acc);
        break;
      case 5:
        std::cout << "Thank you for using SendeMaMe. Goodbye!\n";
        exit(0);
      default:
        std::cout << "Invalid option. Please try again.\n";
        break;
      }
    }
  }
};

int main()
{
  Account acc{};
  CreateAccount createAcc{};
  MainMenu mainMenu{};

  std::cout << "=========== SendeMaMe Mobile Money ==============";

  while (!acc.isRegistered)
  {
    std::cout << "\nYou need an account to continue.\n"
              << "1. Create Account.\n"
              << "2. Exit \n"
              << "Choice: ";

    int choice{};
    if (choice == 2)
    {
      std::cout << "Exiting...";
      return 0;
    }
    else if (choice == 1)
    {
      acc = createAcc.create(acc);
    }
  }
}