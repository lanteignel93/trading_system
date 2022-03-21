### FINM32700 - Advance Computing for Finance
## Final Project
## University of Chicago


### Author: Laurent Lanteigne
### ID: 12279717

import pandas as pd
import os
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import quandl as q

quandl_key = os.environ.get('QUANDL_API_KEY')
plt.style.use('ggplot')

spy = q.get('EOD/' + "SPY" + ".11", start_date='2014-01-30', end_date='2021-05-19', api_key=quandl_key)
spy['ret'] = spy['Adj_Close'].apply(np.log).diff()
spy = spy.dropna()
# Store in csv to use in C++
spy.to_csv('spy_ret.csv')
spy['10ma'] = spy['Adj_Close'].rolling(10, min_periods=1).mean()
spy['25ma'] = spy['Adj_Close'].rolling(25, min_periods=1).mean()
spy['pos'] = np.where(spy['10ma'] > spy['25ma'], 1, 0)
spy['signal'] = spy['pos'].diff().fillna(0)

initial_capital = float(100000)

positions = pd.DataFrame(data=None, index=spy.index).fillna(0)
positions['spy'] = spy['pos'] * 100
portfolio = positions.multiply(spy['Adj_Close'], axis=0)
portfolio['holdings'] = (positions.multiply(spy['Adj_Close'], axis=0)).sum(axis=1)
pos_diff = positions.diff()
portfolio['cash'] = initial_capital - (pos_diff.multiply(spy['Adj_Close'], axis=0)).sum(axis=1).cumsum()
portfolio['total'] = portfolio['cash'] + portfolio['holdings']

portfolio['returns'] = portfolio['total'].pct_change()
portfolio['total'].plot(figsize=(16, 10))
plt.title("Portfolio Value Over Time")
plt.ylabel('$')
plt.savefig("portfolio_value.png")
plt.show()


pnl = portfolio['total'].iloc[-1] - portfolio['total'].iloc[0]
print("Strategy Terminal PnL: {:.2f}$".format(pnl))

trade_df = spy[spy['signal'] != 0]
trade_df = trade_df[['Adj_Close', 'signal']]
trade_df['signal'] = trade_df['signal'].map({1:"BUY", -1:"SELL"})
trade_df.to_csv("python_market_output.csv")