  596  firewall list
  597  firewall append --chain INCOME --dport 23 --target DROP
  598  firewall list
  599  firewall insert --index 1 --chain INCOME --dport 25 --target DROP
  600  firewall list
  601  firewall remove --index 1 --chain INCOME 
  602  firewall list
  603  firewall append --chain INCOME --dport 25 --target DROP
  604  firewall list
  605  firewall modify --index 1 --chain INCOME --dport 26 --target DROP
  606  firewall list
  607  firewall flush --chain INCOME
  608  firewall list
  609  history
  610  history|tail -15 >test.sh
