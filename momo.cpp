#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

void returnToMain()
{
  while (true)
  {
    std::cout << "\n0. Exit   1. Back to Main Menu \nChoice: ";
    int choice{};
    std::cin >> choice;

    if (choice == 0)
    {
      std::cout << "\nThank you for using GnG. \nExiting...";
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

void clearInput()
{
  std::cin.clear();
  std::cin.ignore(100, '\n');
}

// checks that contact is exactly 10 digits
bool isValidContact(std::string contact)
{
  if (contact.length() != 10)
    return false;
  for (int i = 0; i < 10; i++)
  {
    if (contact[i] < '0' || contact[i] > '9')
      return false;
  }
  return true;
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
  time_t cashOutTime{}; // stores when cash out was allowed

  bool verifyPin(int enteredPin)
  {
    return enteredPin == momoPin;
  }

  // checks if cash out is still within 2 minutes
  bool isCashOutActive()
  {
    if (!cashOutAllowed)
      return false;

    time_t now = time(0);
    double secondsPassed = difftime(now, cashOutTime);

    if (secondsPassed > 120) // 120 seconds = 2 minutes
    {
      cashOutAllowed = false; // auto disable
      return false;
    }
    return true;
  }
};

class CreateAccount
{
public:
  Account create(Account acc)
  {
    std::cout << "\n============= Create Account ==============\n";

    // keep asking until valid 10-digit contact
    while (true)
    {
      std::cout << "\nEnter your phone number (10 digits): ";
      std::cin >> acc.contact;
      if (isValidContact(acc.contact))
        break;
      std::cout << "Invalid number. Must be exactly 10 digits. Try again.\n";
    }
    clearInput();

    std::cout << "Enter the name for your account: ";
    std::getline(std::cin, acc.name);

    int pin1{}, pin2{};
    std::cout << "\nCreate a 4-digit GnG PIN: ";
    std::cin >> pin1;
    std::cout << "Confirm the GnG PIN: ";
    std::cin >> pin2;

    if (pin1 != pin2)
    {
      std::cout << "\nPINs do not match. Try again one more time.\n\n";
      std::cout << "Create a 4-digit GnG PIN: ";
      std::cin >> pin1;
      std::cout << "Confirm the GnG PIN: ";
      std::cin >> pin2;

      if (pin1 != pin2)
      {
        std::cout << "PINs do not match again. Account creation cancelled.\n";
        returnToMain();
        return acc;
      }
    }

    acc.momoPin = pin1;
    acc.balance = 5000.0;
    acc.isRegistered = true;

    std::cout << "\nAccount created successfully! Welcome, " << acc.name << ".\n";
    std::cout << "You have received a starting bonus of GHS " << acc.balance << ".\n";
    returnToMain();
    return acc;
  }
};

class TransferMoney
{
private:
  const double fee_rate = 0.01;

  // helper to get a valid 10-digit contact
  std::string getValidContact(std::string prompt)
  {
    std::string contact;
    while (true)
    {
      std::cout << prompt;
      std::cin >> contact;
      if (contact == "0")
        return "0";
      if (isValidContact(contact))
        return contact;
      std::cout << "Invalid number. Must be exactly 10 digits. Try again.\n";
    }
  }

public:
  Account transfer(Account acc)
  {
    std::cout << "============= Transfer Money ===============\n";

    std::string recipient = getValidContact("Enter recipient contact (0 to go back): ");
    if (recipient == "0")
      return acc;

    std::string rp2;
    std::cout << "Confirm recipient contact: ";
    std::cin >> rp2;

    if (recipient != rp2)
    {
      std::cout << "Contacts do not match. Try again.\n";
      recipient = getValidContact("Enter recipient contact: ");
      if (recipient == "0")
        return acc;

      std::cout << "Confirm recipient contact: ";
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
              << "\n\nEnter GnG PIN to confirm or 2 to cancel: ";

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
        std::cout << "Exiting...\n";
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
      acc.cashOutTime = time(0); // record the time cash out was enabled
      std::cout << "Cash out allowed. You have 2 minutes to withdraw.\n";
    }
    else
    {
      std::cout << "Cash out not allowed.\n";
    }
    returnToMain();
    return acc;
  }
};

class WithdrawCash
{
public:
  Account withdraw(Account acc)
  {
    std::cout << "\n========== Withdraw Cash ==========\n";

    // check if cash out is active and within 2 minutes
    if (!acc.isCashOutActive())
    {
      std::cout << "Cash out is not enabled or has expired.\n";
      std::cout << "Go to Allow Cash Out first and you will have 2 minutes to withdraw.\n";
      returnToMain();
      return acc;
    }

    double amount{};
    std::cout << "Enter amount to withdraw (GHS): ";
    std::cin >> amount;

    if (amount <= 0)
    {
      std::cout << "Invalid amount.\n";
      returnToMain();
      return acc;
    }

    if (acc.balance < amount)
    {
      std::cout << "Insufficient balance. Withdrawal failed.\n";
      returnToMain();
      return acc;
    }

    std::cout << "\nYou are about to withdraw GHS " << amount
              << ".\nEnter GnG PIN to confirm or 2 to cancel: ";

    int input{};
    std::cin >> input;
    if (input == 2)
    {
      std::cout << "Withdrawal cancelled.\n";
      returnToMain();
      return acc;
    }

    if (!acc.verifyPin(input))
    {
      std::cout << "\nIncorrect PIN. One more attempt or enter 2 to cancel: ";
      std::cin >> input;
      if (input == 2 || !acc.verifyPin(input))
      {
        std::cout << "\nWithdrawal cancelled.\n";
        returnToMain();
        return acc;
      }
    }

    // check again in case 2 minutes passed while user was entering PIN
    if (!acc.isCashOutActive())
    {
      std::cout << "\nCash out window expired. Withdrawal cancelled.\n";
      returnToMain();
      return acc;
    }

    acc.balance -= amount;
    acc.cashOutAllowed = false; // disable cash out after successful withdrawal
    std::cout << "\nWithdrawal of GHS " << amount << " successful!\n";
    std::cout << "New balance: GHS " << acc.balance << "\n";
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
  WithdrawCash withdrawService{};
  MyWallet walletService{};

public:
  void menu(Account &acc)
  {
    while (true)
    {
      std::cout << "\n\n-------------- Welcome to GnG --------------\n"
                << "Logged in as: " << acc.name << "\n"
                << "Choose from the following options:\n"
                << "1. Transfer Money\n"
                << "2. MomoPay\n"
                << "3. Allow Cash Out\n"
                << "4. Withdraw Cash\n"
                << "5. My Wallet\n"
                << "6. Exit\n"
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
        acc = withdrawService.withdraw(acc);
        break;
      case 5:
        acc = walletService.open(acc);
        break;
      case 6:
        std::cout << "\nThank you for using GnG. Exiting...\n";
        exit(0);
      default:
        std::cout << "\nInvalid option. Please try again.\n";
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

  std::cout << "=========== GnG Mobile Money ==============\n";

  while (!acc.isRegistered)
  {
    std::cout << "\nYou need an account to continue.\n"
              << "1. Create Account\n"
              << "2. Exit\n"
              << "Choice: ";

    int choice{};
    std::cin >> choice;

    if (choice == 2)
    {
      std::cout << "\nExiting...";
      return 0;
    }
    else if (choice == 1)
    {
      acc = createAcc.create(acc);
    }
    else
    {
      std::cout << "\nInvalid Option. Try again.";
    }
  }

  mainMenu.menu(acc);

  return 0;
}