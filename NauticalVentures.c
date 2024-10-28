#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
 *The name of the boat (up to 127 characters long, not containing a comma)
The length in feet - up to 100' (hint: choose a suitable data type)
Which type of place (slip/land/trailor/storage) (hint: use an enum like this)
Depending on the type, extra relevant information (use a union):
Slip - the slip number 1-85
Land - the bay letter, A-Z
Trailor - the trailor license tag
Storage - the storage space number 1-50
The amount of money owed to the marina
 */

typedef char String[128];

typedef enum {
    slip,
    land,
    trailor,
    storage,
    no_place
} PlaceType;

typedef union {
    short int slipNumber;
    char bayLetter;
    char Tag[6];
    short int storageNumber;
} ExtraInfo;

typedef struct {
    String name;
    short int length;
    PlaceType place;
    ExtraInfo extra;
    float money;
} Boat;

//----Convert a string to a place
PlaceType StringToPlaceType(char * PlaceString) {
    for (int i = 0; PlaceString[i];i++){
        PlaceString[i] = tolower(PlaceString[i]);
    }
    if (!strcmp(PlaceString,"slip")) {
        return(slip);
    }
    if (!strcmp(PlaceString,"land")) {
        return(land);
    }
    if (!strcmp(PlaceString,"trailor")) {
        return(trailor);
    }
    if (!strcmp(PlaceString,"storage")) {
        return(storage);
    }
    return no_place;
}

char * PlaceToString(PlaceType Place) {

    switch (Place) {
        case slip:
            return("slip");
        case land:
            return("land");
        case trailor:
            return("trailor");
        case storage:
            return("storage");
        case no_place:
            return("no_place");
        default:
            printf("How the faaark did I get here?\n");
            exit(EXIT_FAILURE);
            break;
    }
}

ExtraInfo PlaceToExtraInfo(PlaceType place,char *extraInfoString) {
    ExtraInfo extra;
    switch (place){
        case slip:
	    extra.slipNumber = (short int) atoi(extraInfoString);
	    break;
	case land:
	    extra.bayLetter = extraInfoString[0];
	    break;
	case trailor:
	    strncpy(extra.Tag, extraInfoString,sizeof(extra.Tag) - 1);
	    break;
	case storage:
	    extra.storageNumber = (short int)atoi(extraInfoString);
	    break;
	default:
	    break;
    }
    return extra;
}

void printExtraInfo(PlaceType place, ExtraInfo extra) {
    switch (place) {
        case slip:
            printf("#%d", extra.slipNumber);
            break;
        case land:
            printf("%c", extra.bayLetter);
            break;
        case trailor:
            printf("%s", extra.Tag);
            break;
        case storage:
            printf("#%d", extra.storageNumber);
            break;
            }
}

void addBoat(Boat *marina[], Boat newBoat, int *count){
    
    // last index of the array
    int i = *count -1;
    for (int j = 0; newBoat.name[j];j++){
        newBoat.name[j] = tolower(newBoat.name[j]);
    }
    // while the boat name is greater than the index name, decrement
    // shift all entries down until we get to the New Boat's place
    while (i>=0 && strcmp(marina[i]->name, newBoat.name) > 0) {
        marina[i+1] = marina[i];
	 i--;
    }
    // once we get to the index that is smaller than newBoat, go back to the index in front of it
    marina[i+1] = malloc(sizeof(Boat));
    *marina[i+1] = newBoat;
    (*count)++;

}


int main(){
    int count = 0;
    Boat *marina[120];
    int exit = 1;
    while(exit){
    char response;
    printf("(I)nventory, (A)dd, (R)emove, (P)ayment, (M)onth, e(X)it :\n");
    scanf(" %c", &response);
    if (response == 'i'){
        response = 'I';
    }
    if (response == 'a'){
        response = 'A';
    }
    if (response == 'r'){
        response = 'R';
    }
    if (response == 'p'){
        response = 'P';
    }
    if (response == 'm'){
        response = 'M';
    }
    if (response == 'x'){
        response = 'X';
    }
    switch(response){
	case 'I':
            for (int i = 0; i < count; i++) {
	        Boat tempBoat = *marina[i];
	        printf("%-127s %3d' %7s ", tempBoat.name,tempBoat.length,PlaceToString(tempBoat.place));
		printExtraInfo(tempBoat.place,tempBoat.extra);
		printf(" $%7.2f\n",tempBoat.money);
	    }
	    break;
        case 'A':
	    Boat newBoat;
	    String name;
            short int length;
            ExtraInfo extraTemp;
            float money;
	    printf("Enter the name of the boat, length, storage place, storage information, and money owed to the marina, in one string separated by commmas:\n");
	    char tempStorage[10];
	    char tempExtra[10];
	    scanf("%[^,],%hd,%s,%s,%f",newBoat.name, &newBoat.length, tempStorage,tempExtra,&newBoat.money);
	    PlaceType placeTemp = StringToPlaceType(tempStorage);
	    newBoat.place = placeTemp;
            extraTemp = PlaceToExtraInfo(newBoat.place,tempExtra);
	    newBoat.extra = extraTemp;
	    addBoat(marina, newBoat, &count);
	    break;
        case 'R':
	    String toBeRemoved;
	    printf("Please enter the boat name:");
	    scanf("%s", toBeRemoved);
	    for (int z = 0; toBeRemoved[z];z++){
	        toBeRemoved[z] = tolower(toBeRemoved[z]);
	    }
	    for (int j = 0; j < count; j++) {
	        if (strcasecmp(marina[j]->name, toBeRemoved) == 0) {
		    free(marina[j]);
		    for(int m=j; m < count -1; m++){
		        marina[m] = marina[m+1];
		    }
		    marina[count-1] = NULL;
		    count--;
		    break;
		}
	    }
	    printf("No boat with that name");
	    break;
        case 'P':
	    String toBePaid;
            printf("Please enter the boat name:");
            scanf("%s", toBePaid);
            float payment;
            printf("Please enter the amount to be paid:");
            scanf("%f", &payment);
            for (int k = 0; k < count; k++) {
		for (int s = 0; marina[k]->name[s];s++){
                    marina[k]->name[s] = tolower(marina[k]->name[s]);
		}
		for (int s = 0; toBePaid[s];s++){
                    toBePaid[s] = tolower(toBePaid[s]);
		}
	        if (strcmp(marina[k]->name, toBePaid) == 0) {
		    float balance = marina[k]->money;
		    if (balance < payment){
	                printf("That is more than the amount owed, %.2f\n", balance);
		        break;
		    }
		    else {
		        balance = balance - payment;
			marina[k]->money = balance;
			break;
		    }
		}
	    }
	    break;
        case 'M':
            for (int l = 0; l < count; l++) {
                Boat tempBoat = *marina[l];
		float balance = tempBoat.money;
		if (tempBoat.place == slip){
		    float charge = 12.5 * (tempBoat.length);
		    balance = balance + charge;
		    marina[l]->money = balance;
		}
		if (tempBoat.place == land){
                    float charge = 14.0 * (tempBoat.length);
                    balance = balance + charge;
		    marina[l]->money = balance;
                }
		if (tempBoat.place == trailor){
                    float charge = 25.0 * (tempBoat.length);
                    balance = balance + charge;
                    marina[l]->money = balance;
                }
		if (tempBoat.place == storage){
                    float charge = 11.2 * (tempBoat.length);
                    balance = balance + charge;
                    marina[l]->money = balance;
		}
	    }
	    break;
        case 'X':
	        printf("Exiting the Boat Management System");
		exit = 0;
	        break;
	default:
            printf("Please enter a valid option!\n");
	    break;
	}
    }
}    
