import yfinance as yf
import pandas as pd

# Core assets
oracle = yf.download("ORCL", period="6mo")
nasdaq = yf.download("^IXIC", period="6mo")
nvda = yf.download("NVDA", period="6mo")

# New macro factors
europe = yf.download("^STOXX50E", period="6mo")  # Europe index
oil = yf.download("CL=F", period="6mo")          # WTI crude

df = pd.DataFrame()

df["oracle"] = oracle["Close"].pct_change()
df["nasdaq"] = nasdaq["Close"].pct_change()
df["capex"] = nvda["Close"].pct_change()
df["europe"] = europe["Close"].pct_change()
df["oil"] = oil["Close"].pct_change()

df = df.dropna()

# Smooth signals
df["oracle"] = df["oracle"].rolling(5).mean()
df["capex"] = df["capex"].rolling(5).mean()

df = df.dropna()

df.to_csv("macro_data_v2.csv", index=False)

print("✅ macro_data_v2.csv generated")
