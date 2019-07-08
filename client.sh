#!/bin/bash
set -e

#general variables
scriptName="client.sh"
curlOpt="-sSf" #variable for disable curl errors when the user gave -q option

#server variables
protocol=""
serverName=""

#options flags
addFlag=0
updateFlag=0
removeFlag=0
showSpecificFlag=0
showAllFlag=0

#options variables
category=""
name=""
price=""
quantity=""
identify=""

# Print a synopsis for option 'help'.
synopsis() {
	echo "NAME
    $scriptName - client side

SYNOPSIS
    api for communication with the storge manager server.

    $scriptName -h|--help
    	Show this synopsis and exit.

    $scriptName -q|--quiet
    	Disable error output.

    $scriptName [-a|--add] [-c|--category] <category name> [-n|--name] <product name> [-p|--price] <ptoduct price> (integer)
			[-v|--quantity] <product quantity> (integer) [-i|--identify] <product identify> (integer)
     	Allows the user to add a new product to the stock.

    $scriptName [-r|--remove] [-i|--identify] <product identify> (integer)
      	Allows the user to remove an existing product from the stock.

    $scriptName [-u|--update] [-i|--identify] <product identify> (integer) [-v|--quantity] <product quantity> (integer)
    	Allows the user to update quantity of an existing product.

    $scriptName [-m|--showSpecific] [-i|--identify] <product identify> (integer)
    	Allows the user to show detailes about specific product in the stock.

    $scriptName [-s|--showAll] 
    	Allows the user to show all the products in the stock."
	exit 0
}

#print the error message and close the script
error() {
	echo "$scriptName: $1" 1>&2
	exit 1
}

add() {
	curl "$curlOpt" "$protocol$serverName/add?category=$1&name=$2&ID=$3&price=$4&quantity=$5"
}

remove() {
	curl "$curlOpt" "$protocol$serverName/remove?ID=$1"
}

update() {
	curl "$curlOpt" "$protocol$serverName/update?ID=$1&quantity=$2"
}

showAll() {
	curl "$curlOpt" "$protocol$serverName/showAll"
}

showSpecific() {
	curl "$curlOpt" "$protocol$serverName/showSpecific?ID=$1"
}

OPTS=$(getopt -n "$scriptName" -o hqaursmc:n:p:v:g:i: -l help,quiet,add,update,remove,showAll,showSpecific,category:,name:,price:,quantity:,productID:,identify: -- "$@")
eval set -- "$OPTS"

#loop trough the options of getopt.
while [ "$1" != "--" ]; do
	case "$1" in
		-h | --help)
			synopsis ;;
		-q | --quiet)
			curlOpt="-sf" # turn off the error output of curl
			exec 2>/dev/null ;;
		-a | --add)
			addFlag=1 ;;
		-u | --update)
			updateFlag=1 ;;
		-r | --remove)
			removeFlag=1 ;;
		-s | --showAll)
			showAllFlag=1 ;;
		-m | --showSpecific)
			showSpecificFlag=1 ;;		
		-c | --category)
			category="$2"; shift ;;
		-n | --name)
			name="$2"; shift ;;
		-p | --price)
			price="$2"; shift ;;
		-v | --quantity)
			quantity="$2"; shift ;;
		-i | --identify)
			identify="$2"; shift ;;
		*)
			error "$scriptName: Some error occured!" ;;
	esac
	shift
done;

shift #shift the '--' parameter.

#if the user doesn't give any option
if [ "$addFlag" = 0 ] && [ "$removeFlag" = 0 ] && [ "$updateFlag" = 0 ] && [ "$showSpecificFlag" = 0 ] && [ "$showAllFlag" = 0 ]; then
	error "You must enter at least one option!"
fi

# if the user choose to add a new product.
if [ "$addFlag" = 1 ]; then 
	# Check if more then one action were tapping -> if there are, show message and end the program.
	if [ "$removeFlag" = 1 ] || [ "$updateFlag" = 1 ] || [ "$showSpecificFlag" = 1 ] || [ "$showAllFlag" = 1 ]; then 
		error "You can not do more then one action!"
	fi

	# Check if some of the paremters were no entered -> if there are, show message and end the program.
	if [ "$category" = "" ]; then error "No category entered!"; fi
	if [ "$name" = "" ]; then error "No name entered!"; fi
	if [ "$identify" = "" ]; then error "No identify entered!"; fi
	if [ "$price" = "" ]; then error "No price entered!"; fi
	if [ "$quantity" = "" ]; then error "No quantity entered!"; fi

	# All the paremters were entered. Start 'add' action.
	add "$category" "$name" "$identify" "$price" "$quantity"
fi

# if the user choose to remove an existing product.
if [ "$removeFlag" = 1 ]; then 
	# Check if more then one action were tapping -> if there are, show message and end the program.
	if [ "$addFlag" = 1 ] || [ "$updateFlag" = 1 ] || [ "$showSpecificFlag" = 1 ] || [ "$showAllFlag" = 1 ]; then 
		error "You can not do more then one action!"
	fi

	# Check if 'identify' paremter was no entered -> if not, show message and end the program.
	if [ "$identify" = "" ]; then error "No identify entered!"; fi

	# Check if illegal options were tapping -> if there are, show message and end the program.
	if [ "$category" != "" ] || [ "$name" != "" ] || [ "$price" != "" ] || [ "$quantity" != "" ]; then 
		error "Invalid parameter for 'remove' option!"
	fi
	
	# Start 'remove' action.
	remove "$identify"
fi

# if the user choose to update quantity of an existing product.
if [ "$updateFlag" = 1 ]; then 
	# Check if more then one action were tapping -> if there are, show message and end the program.
	if [ "$removeFlag" = 1 ] || [ "$addFlag" = 1 ] || [ "$showSpecificFlag" = 1 ] || [ "$showAllFlag" = 1 ]; then 
		error "You can not do more then one action!"
	fi
	# Check if some of the paremters were no entered -> if there are, show message and end the program.
	if [ "$identify" = "" ]; then error "No identify entered!"; fi
	if [ "$quantity" = "" ]; then error "No quantity entered"; fi

	# Check if illegal options were tapping -> if there are, show message and end the program.
	if [ "$category" != "" ] || [ "$name" != "" ] || [ "$price" != "" ]; then 
		error "Invalid parameter for 'update' option!"
	fi
	
	# Start 'update' action.
	update "$identify" "$quantity"
fi

# if the user choose to show details about specific product.
if [ "$showSpecificFlag" = 1 ]; then 
	# Check if more then one action were tapping -> if there are, show message and end the program.
	if [ "$removeFlag" = 1 ] || [ "$updateFlag" = 1 ] || [ "$addFlag" = 1 ] || [ "$showAllFlag" = 1 ]; then 
		error "You can not do more then one action!"
	fi

	# Check if 'identify' paremter was no entered -> if not, show message and end the program.
	if [ "$identify" = "" ]; then error "No identify entered!"; fi

	# Check if illegal options were tapping -> if there are, show message and end the program.
	if [ "$category" != "" ] || [ "$name" != "" ] || [ "$price" != "" ] || [ "$quantity" != "" ]; then 
		error "Invalid parameter for 'showSpecific' option!"
	fi
	
	# Start 'showSpecific' action.
	showSpecific "$identify"
fi

# if the user choose to show show all the prosucts.
if [ "$showAllFlag" = 1 ]; then 
	# Check if more then one action were tapping -> if there are, show message and end the program.
	if [ "$removeFlag" = 1 ] || [ "$updateFlag" = 1 ] || [ "$addFlag" = 1 ] || [ "$showSpecificFlag" = 1 ]; then 
		error "You can not do more then one action!"
	fi

	# Check if illegal options were tapping -> if there are, show message and end the program.
	if [ "$category" != "" ] || [ "$name" != "" ] || [ "$price" != "" ] || [ "$quantity" != "" ] || [ "$identify" != "" ]; then 
		error "Invalid parameter for 'showAll' option!"
	fi
	
	# Start 'showAll' action.
	showAll
fi

