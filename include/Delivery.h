#pragma once


namespace dei {

struct Delivery;
typedef std::shared_ptr< Delivery >  DeliveryPtr;
typedef std::unique_ptr< Delivery >  DeliveryUPtr;
typedef std::vector< DeliveryPtr >  listDeliveryPtr_t;

}




namespace std {

ostream& operator<<( std::ostream& out, const dei::Delivery& );
ostream& operator<<( std::ostream& out, const std::vector< dei::DeliveryPtr >& );

} // std





namespace dei {


/**
* Доставка.
*
* @see http://orders.novaposhta.ua/api.php?todo=api_form
*/
struct Delivery {
    // Данные получателя (загружаются из файла)
    // @see data/1.csv
    std::string date;
    std::string number;     //+
    std::string item;       //+
    std::string name;       //+
    std::string phone;      //+
    std::string postcode;
    std::string city;       //+
    std::string address;    //+
    std::string total;
    std::string totalClear; //+
    std::string note;       //+


    // декларируемая стоимость
    std::string declareCost;

    // номер отделения для доставки получателю
    std::string warehouse;

    // тип упаковки, здесь - "Коробка"
    std::string packType;

    // наличный (1) или безналичный (2) расчёт
    std::string payType;

    // кто платит: получатель (0), отправитель (1), третье лицо (3)
    std::string payer;

    // дополнительная информация (в доп. услугах, "Документы Ц1П XXX"
    std::string additionalInfo;

    // описание доставки
    // @see Поле 'ignoreItem'
    std::string description;

    // тип обратной доставки
    // [1 - документи | 2 - гроші | 3 - піддони (тара) | 4 - товар | 5 - інше]
    std::string redeliveryType;

    // город плательщика обратной доставки
    std::string redeliveryPaymentCity;

    // плательщик обратной доставки
    // [1 - відправник | 2 - отримувач]
    std::string redeliveryPaymentPayer;

    // Эти поля заполняются по умолчанию
    // @todo Вынести в файл концигурации.
    // @todo fine Все поля переписать через Key.
    std::string senderCity;
    std::string senderCompany;
    std::string senderAddress;
    std::string senderContact;
    std::string senderPhone;

    // оплата за дорученням, якщо окремо підписаний договір
    std::string deliveryAmount;

    // що саме в зворотній доставці
    std::string deliveryInOut;

    // масса и габариты
    std::string weight;
    std::string length;
    std::string width;
    std::string height;

    // описание каждого места доставки
    // @see Поле 'ignoreItem'
    std::string containerDescription;


    // данные аутентификации
    std::string oauth;


    // Полученный от "Новой почты" номер оформленной заявки
    std::string novaposhtaNumber;


    // Если 'true', вместо описания места будут оставлены значения полей
    // 'description' и 'cont_description' по умолчанию.
    bool ignoreItem;

    // Эта фраза добавляется к полю 'note' как только в поле встретилось слово 'двер'
    // Пример: "доставка до двери" отправится как "доставка до двери СОЗВОНИТЬСЯ".
    std::string screamDoorNote;


    // В файл для импорта к каждой заявке добавляется (1) тип и (2) сумма доставки.
    //typedef std::pair< std::string, std::string >  typeCostDelivery_t;
    typedef std::vector< std::string >  typeCostDelivery_t;
    typeCostDelivery_t doorDoorDelivery;
    typeCostDelivery_t doorStockDelivery;
    typeCostDelivery_t stockDoorDelivery;
    typeCostDelivery_t stockStockDelivery;



    inline Delivery( const std::string& initFile ) {
        initDefault( this, initFile );
    }




    virtual inline ~Delivery() {
    }



    /**
    * @return Один из типов доставки, опираясь на данные из заявки.
    *         В нашем случае доставка бывает след. типов:
    *           - stock-door
    *           - stock-stock
    */
    std::string typeDelivery() const;



    /**
    * Загружает доставки из указанного файла в 'r'.
    * Пример файла - см. data/1.csv
    *
    * @param configure Файл конфигурации, с которым будет создана доставка.
    */
    static void load(
        listDeliveryPtr_t& r,
        const std::string& source,
        const std::string& configure
    );





protected:
    /**
    * Наполняет доставку значениями по умолчанию.
    */
    static void initDefault(
        Delivery* d,
        const std::string&  initFile
    );



    /**
    * @return Поле, очищенное от крайних пробелов.
    */
    static std::string correctField( const std::string& s );



    /**
    * @return Поле с названием города, очищенное от мусора.
    */
    static std::string correctCity( const std::string& s );

};



} // dei
