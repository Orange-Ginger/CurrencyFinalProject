#include <gtest/gtest.h>
#include "currency.h"
#include "currency_parser.h"

TEST(CryptoCurrencyTest, ConstructionAndGetters) {
    std::vector<std::string> allowed = {"United States", "Japan"};
    CryptoCurrency btc("BTC", "₿", 105767, 0.02, 1.35, allowed);

    EXPECT_EQ(btc.getCode(), "BTC");
    EXPECT_EQ(btc.getSymbol(), "₿");
    EXPECT_EQ(btc.getType(), "Crypto");
    EXPECT_DOUBLE_EQ(btc.getRate(), 105767);
    EXPECT_DOUBLE_EQ(btc.getTaxRate(), 0.02);
    EXPECT_DOUBLE_EQ(btc.getVolatility(), 1.35);
    EXPECT_TRUE(btc.canBeUsedIn("Japan"));
    EXPECT_FALSE(btc.canBeUsedIn("France"));
}

TEST(FiatCurrencyTest, ConstructionAndGetters) {
    std::vector<std::string> allowed = {"United States"};
    FiatCurrency usd("USD", "$", 1.0, 0.005, 0.023, allowed);

    EXPECT_EQ(usd.getCode(), "USD");
    EXPECT_EQ(usd.getSymbol(), "$");
    EXPECT_EQ(usd.getType(), "Fiat");
    EXPECT_DOUBLE_EQ(usd.getRate(), 1.0);
    EXPECT_DOUBLE_EQ(usd.getTaxRate(), 0.005);
    EXPECT_DOUBLE_EQ(usd.getInflationRate(), 0.023);
    EXPECT_TRUE(usd.canBeUsedIn("United States"));
    EXPECT_FALSE(usd.canBeUsedIn("Japan"));
}

TEST(MagicCurrencyTest, ConstructionAndGetters) {
    MagicCurrency arsh("ARSH", "✨", 100.0, 0.05, 7, "Fireball", "Arcadia");

    EXPECT_EQ(arsh.getCode(), "ARSH");
    EXPECT_EQ(arsh.getSymbol(), "✨");
    EXPECT_EQ(arsh.getType(), "Magic");
    EXPECT_DOUBLE_EQ(arsh.getRate(), 100.0);
    EXPECT_DOUBLE_EQ(arsh.getTaxRate(), 0.05);
    EXPECT_EQ(arsh.getRarityLevel(), 7);
    EXPECT_EQ(arsh.getIncantation(), "Fireball");
    EXPECT_TRUE(arsh.canBeUsedIn("Arcadia"));
}

TEST(CurrencyConverterTest, AddAndGetCurrency) {
    CurrencyConverter converter;
    auto usd = std::make_shared<FiatCurrency>("USD", "$", 1.0, 0.005, 0.023, std::vector<std::string>{"United States"});
    converter.addCurrency(usd);

    auto retrieved = converter.getCurrency("USD");
    EXPECT_EQ(retrieved->getCode(), "USD");
}

TEST(CurrencyConverterTest, ConvertFiatToFiat) {
    CurrencyConverter converter;
    auto usd = std::make_shared<FiatCurrency>("USD", "$", 1.0, 0.005, 0.023, std::vector<std::string>{"United States"});
    auto eur = std::make_shared<FiatCurrency>("EUR", "€", 1.14, 0.01, 0.019, std::vector<std::string>{"France"});
    converter.addCurrency(usd);
    converter.addCurrency(eur);

    double result = converter.convert("USD", "EUR", 10.0);
    EXPECT_NEAR(result, 10.0 * 1.0 / 1.14, 1e-6);
}

TEST(CurrencyConverterTest, ConvertCryptoToFiat) {
    CurrencyConverter converter;
    auto btc = std::make_shared<CryptoCurrency>("BTC", "₿", 105767, 0.02, 1.35, std::vector<std::string>{"United States"});
    auto usd = std::make_shared<FiatCurrency>("USD", "$", 1.0, 0.005, 0.023, std::vector<std::string>{"United States"});
    converter.addCurrency(btc);
    converter.addCurrency(usd);

    double result = converter.convert("BTC", "USD", 2.0);
    EXPECT_NEAR(result, 2.0 * 105767 / 1.0, 1e-6);
}

TEST(CurrencyConverterTest, ThrowsOnUnknownCurrency) {
    CurrencyConverter converter;
    auto usd = std::make_shared<FiatCurrency>("USD", "$", 1.0, 0.005, 0.023, std::vector<std::string>{"United States"});
    converter.addCurrency(usd);

    EXPECT_THROW(converter.getCurrency("EUR"), std::runtime_error);
    EXPECT_THROW(converter.convert("USD", "EUR", 1.0), std::runtime_error);
    EXPECT_THROW(converter.convert("EUR", "USD", 1.0), std::runtime_error);
}

TEST(FiatCurrencyTest, ApplyTaxOrFee) {
    std::vector<std::string> allowed = {"United States"};
    FiatCurrency usd("USD", "$", 1.0, 0.01, 0.02, allowed);

    EXPECT_DOUBLE_EQ(usd.applyTaxOrFee(50), 0.5);
    EXPECT_DOUBLE_EQ(usd.applyTaxOrFee(500), 500 * 0.01 * 0.95);
    EXPECT_DOUBLE_EQ(usd.applyTaxOrFee(5000), 5000 * 0.01 * 0.9);
    EXPECT_DOUBLE_EQ(usd.applyTaxOrFee(20000), 20000 * 0.01 * 0.8);
}

TEST(CryptoCurrencyTest, ApplyTaxOrFee) {
    std::vector<std::string> allowed = {"United States"};
    CryptoCurrency btc("BTC", "₿", 105767, 0.02, 1.35, allowed);

    EXPECT_DOUBLE_EQ(btc.applyTaxOrFee(100), 2.0);
}

TEST(CryptoCurrencyTest, FluctuateAndStability) {
    std::vector<std::string> allowed = {"United States"};
    CryptoCurrency btc("BTC", "₿", 100, 0.02, 1.35, allowed);

    double oldRate = btc.getRate();
    btc.fluctuate();
    EXPECT_NE(btc.getRate(), oldRate);
    EXPECT_FALSE(btc.isStable());
}

TEST(MagicCurrencyTest, ApplyTaxOrFee) {
    MagicCurrency arsh("ARSH", "✨", 100.0, 0.05, 7, "Fireball", "Arcadia");
    double fee = arsh.applyTaxOrFee(100);
    EXPECT_GT(fee, 0.0);
}

TEST(MagicCurrencyTest, CanBeUsedInRealmOrigin) {
    MagicCurrency arsh("ARSH", "✨", 100.0, 0.05, 7, "Fireball", "Arcadia");
    EXPECT_TRUE(arsh.canBeUsedIn("Arcadia"));
}

TEST(ParserTest, SplitCSVLine) {
    auto fields = splitCSVLine("USD,$,1.0,0.005,0.023,\"United States,Canada\"", ',');
    EXPECT_EQ(fields[0], "USD");
    EXPECT_EQ(fields[5], "United States,Canada");
}

TEST(ParserTest, SplitCountries) {
    auto countries = splitCountries("United States,Canada");
    EXPECT_EQ(countries.size(), 2);
    EXPECT_EQ(countries[0], "United States");
    EXPECT_EQ(countries[1], "Canada");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}