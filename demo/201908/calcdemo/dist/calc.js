import * as BigNumInterface from "./nakasendo/BigNumInterface.js";
export class Calculator {
    constructor() {
        this.currentValue = new BigNumInterface.BigNumber();
        this.curretValue = 0;
        // handle GUI button clicks
        this.operation = null;
        this.previousButton = null;
        // ******************************************************************************************************************
        // update info panel with digit count and adjust input text size relative to digit count
        this.displayInfo = document.querySelector('.info');
        this.displayInput = document.querySelector('input.display');
    }
    /* Updates current value by performing big number operation on current value and new entry */
    performOperation(operation, newEntry) {
        if (operation == "add") {
            this.currentValue = this.currentValue.add(new BigNumInterface.BigNumber(newEntry));
        }
        else if (operation == "subtract") {
            this.currentValue = this.currentValue.sub(new BigNumInterface.BigNumber(newEntry));
        }
        else if (operation == "multiply") {
            this.currentValue = this.currentValue.multiply(new BigNumInterface.BigNumber(newEntry));
        }
        else if (operation == "divide") {
            this.currentValue = this.currentValue.divide(new BigNumInterface.BigNumber(newEntry));
        }
        return this.currentValue;
    }
    /** generates 1024bit random number */
    generateRandomNumber() {
        return new BigNumInterface.BigNumber().random(1024);
    }
    onButtonClick(button, entry) {
        let result = new BigNumInterface.BigNumber();
        if (button == "result") {
            // if we have operation than compute it otherwise just return current value
            if (this.operation != null)
                result = this.performOperation(this.operation, entry);
            else
                result = this.currentValue;
            this.operation = null;
        }
        else if (button == "clear") {
            // clear state
            this.operation = null;
            this.currentValue = new BigNumInterface.BigNumber();
        }
        else if (button == "random") {
            // just return new random number
            result = this.generateRandomNumber();
        }
        else if (this.isOperation(button)) {
            // if we have previous operation than compute it before storing new operation
            if (this.operation != null) {
                result = this.performOperation(this.operation, entry);
            }
            else {
                this.currentValue = new BigNumInterface.BigNumber(entry);
                result = this.currentValue;
            }
            this.operation = button;
        }
        else if (button == "backspace") {
            entry = entry.replace(/^0+/, '');
            if (entry.length > 0)
                entry = entry.substr(0, entry.length - 1);
            result = new BigNumInterface.BigNumber(entry.length == 0 ? "0" : entry);
        }
        else {
            // check if previous button was operation - in that case we are entering new number
            if (this.isOperation(this.previousButton) || this.previousButton == "result")
                entry = '';
            // remove leading zero(s)
            entry = entry.replace(/^0+/, '');
            result = new BigNumInterface.BigNumber(entry + button);
        }
        // update digit count and entry font size
        this.updateInfoAndFontSize(result.toString());
        // store previous button
        this.previousButton = button;
        return result.toString();
    }
    // ******************************************************************************************************************
    // handle keyboard (redirect to onButtonClick)
    onKeyDown(e, entry) {
        if (e.keyCode == 13 || e.key == "=") {
            return this.onButtonClick("result", entry);
        }
        else if (e.keyCode == 8) {
            return this.onButtonClick("backspace", entry);
        }
        else if (e.keyCode == 27 || e.key.toLowerCase() == "c") {
            return this.onButtonClick("clear", entry);
        }
        else if (e.key.toLowerCase() == "r") {
            return this.onButtonClick("random", entry);
        }
        else if (this.isOperation(this.keyToOperation(e.key))) {
            return this.onButtonClick(this.keyToOperation(e.key), entry);
        }
        return entry;
    }
    onKeyPress(e, entry) {
        if (this.isNumber(e.key)) {
            return this.onButtonClick(e.key, entry);
        }
        return entry;
    }
    updateInfoAndFontSize(entry) {
        if (this.displayInput) {
            if (entry.length > 185) {
                this.displayInput.className = "display size185";
            }
            else if (entry.length > 90) {
                this.displayInput.className = "display size90";
            }
            else if (entry.length > 60) {
                this.displayInput.className = "display size60";
            }
            else if (entry.length > 40) {
                this.displayInput.className = "display size30";
            }
            else {
                this.displayInput.className = "display";
            }
        }
        if (this.displayInfo) {
            entry = entry.replace(/^0+/, '');
            entry = entry.replace(/\-+/, '');
            this.displayInfo.textContent = "Number of digits: " + entry.length;
        }
    }
    // ******************************************************************************************************************
    // returns true if input is add, substract, multiply or divide operation  
    isOperation(button) {
        return button == "add" ||
            button == "subtract" ||
            button == "multiply" ||
            button == "divide";
    }
    // ******************************************************************************************************************
    // returns true if value is number
    isNumber(value) {
        return ((value != null) && !isNaN(Number(value.toString())));
    }
    // ******************************************************************************************************************
    // converts keyboard key to operation if applicable
    keyToOperation(value) {
        switch (value) {
            case "+": return "add";
            case "-": return "subtract";
            case "*": return "multiply";
            case "/": return "divide";
        }
        return "";
    }
}
