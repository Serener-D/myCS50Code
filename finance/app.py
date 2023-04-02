import os

# export API_KEY=pk_5cc6d5c3d6834f5d82de0783e3b0d97f

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    current_user_id = session["user_id"]
    current_user_cash = db.execute("SELECT cash FROM users WHERE id=?", current_user_id)[0]["cash"]
    grand_total = current_user_cash
    all_user_stocks = db.execute("SELECT symbol, amount FROM owned_stocks WHERE user_id=?", current_user_id)

    for i in range(len(all_user_stocks)):
        stock_price = lookup(all_user_stocks[i]["symbol"])["price"]
        sum = stock_price * all_user_stocks[i]["amount"]
        all_user_stocks[i]["sum"] = sum
        all_user_stocks[i]["price"] = stock_price
        grand_total = grand_total + sum
    return render_template("index.html", grand_total=grand_total, current_user_cash=current_user_cash, all_user_stocks=all_user_stocks)


@app.route("/gimme", methods=["GET", "POST"])
@login_required
def gimme():
    """Buy shares of stock"""
    if request.method == "POST":

        amount = float(request.form.get("amount"))
        if amount < 0:
            return apology("Input is not a positive number", 400)

        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        db.execute("UPDATE users SET cash=? WHERE id=?", user_cash[0]["cash"] + amount, session["user_id"])

        return index()
    else:
        return render_template("gimme.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Provided symbol is blank", 400)

        stock_info = lookup(symbol)
        if not stock_info:
            return apology("No information for provided symbol", 400)

        number_of_shares = request.form.get("shares")
        if not number_of_shares.isdigit():
            return apology("You should submit integer number of shares", 400)

        if int(number_of_shares) < 1:
            return apology("Input is not a positive integer", 400)

        stock_price = stock_info["price"]
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        sum = stock_price * int(number_of_shares)
        if sum > user_cash[0]["cash"]:
            return apology("You cannot afford requested number of shares at the current price", 400)

        db.execute("UPDATE users SET cash=? WHERE id=?", user_cash[0]["cash"] - sum, session["user_id"])
        db.execute("INSERT INTO transactions (user_id, transaction_type, symbol, share_price, transaction_amount, share_amount) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], "buy", symbol, stock_price, sum, int(number_of_shares))
        owned_stock = db.execute("SELECT amount FROM owned_stocks WHERE user_id=? AND symbol=?", session["user_id"], symbol)
        if (len(owned_stock) == 0):
            db.execute("INSERT INTO owned_stocks (user_id, symbol, amount) VALUES (?, ?, ?)",
                       session["user_id"], symbol, int(number_of_shares))
        else:
            db.execute("UPDATE owned_stocks SET amount=? WHERE user_id=? AND symbol=?",
                       owned_stock[0]["amount"] + int(number_of_shares), session["user_id"], symbol)

        return index()
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id=?", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Provided symbol is blank", 400)

        result = lookup(symbol)
        if not result:
            return apology("No results for provided symbol", 400)

        return render_template("quoted.html", result=result)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get("username")
        user_row = db.execute("SELECT * FROM users WHERE username = ?", username)
        if not username:
            return apology("Provided username is blank", 400)
        elif len(user_row) != 0:
            return apology("Provided username already exists", 400)

        password = request.form.get("password")
        if not password:
            return apology("Provided password is blank", 400)

        confirmation = request.form.get("confirmation")
        if password != confirmation:
            return apology("Passwords do not match", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
        return render_template("login.html")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    owned_stocks_symbols = db.execute("SELECT symbol, amount FROM owned_stocks WHERE user_id=?", session["user_id"])

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Provided symbol is blank", 400)

        owned_stock_info = dict()
        is_Stock_Owned = False
        for row in owned_stocks_symbols:
            if symbol == row["symbol"]:
                owned_stock_info = row
                is_Stock_Owned = True

        if (is_Stock_Owned == False):
            return apology("You don't own provided stock", 400)

        stock_info = lookup(symbol)
        if not stock_info:
            return apology("No information for provided symbol", 400)

        number_of_shares = request.form.get("shares")

        if not number_of_shares.isdigit():
            return apology("You should submit integer number of shares", 400)

        if int(number_of_shares) < 1:
            return apology("Input is not a positive integer", 400)

        if int(number_of_shares) > owned_stock_info["amount"]:
            return apology("You do not own that many shares of the stock.", 400)

        stock_price = stock_info["price"]
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        sum = stock_price * int(number_of_shares)

        db.execute("UPDATE users SET cash=? WHERE id=?", user_cash[0]["cash"] + sum, session["user_id"])
        db.execute("INSERT INTO transactions (user_id, transaction_type, symbol, share_price, transaction_amount, share_amount) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], "sell", symbol, stock_price, sum, int(number_of_shares))

        print(owned_stock_info["amount"])
        print(number_of_shares)
        print(owned_stock_info["amount"] == number_of_shares)
        if (owned_stock_info["amount"] == int(number_of_shares)):
            db.execute("DELETE FROM owned_stocks WHERE user_id=? AND symbol=?", session["user_id"], symbol)
        else:
            db.execute("UPDATE owned_stocks SET amount=? WHERE user_id=? AND symbol=?",
                       owned_stock_info["amount"] - int(number_of_shares), session["user_id"], symbol)

        return index()
    else:
        return render_template("sell.html", owned_stocks_symbols=owned_stocks_symbols)
