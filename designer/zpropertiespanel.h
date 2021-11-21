#ifndef __ZPROPERTIES_PANEL_H__
#define __ZPROPERTIES_PANEL_H__

class ValueInputWidget : public QWidget
{
    Q_OBJECT
public:
    ValueInputWidget(const QString& name, QWidget* parent = nullptr);

private:
    QSpinBox* m_pSpinBox;
    QLineEdit* m_pLineEdit;
};

class ZPagePropPanel : public QWidget
{
    Q_OBJECT
public:
    ZPagePropPanel(QWidget* parent = nullptr);

private:
    ValueInputWidget* m_pWidth;
    ValueInputWidget* m_pHeight;
};

class ZComponentPropPanel : public QWidget
{
    Q_OBJECT
public:
    ZComponentPropPanel(QWidget* parent = nullptr);

private:
    ValueInputWidget* m_pWidth;
    ValueInputWidget* m_pHeight;
    ValueInputWidget* m_pX;
    ValueInputWidget* m_pY;
};

class ZElementPropPanel : public QWidget
{
    Q_OBJECT
public:
    ZElementPropPanel(QWidget* parent = nullptr);

private:
    QLabel* m_pAsset;
    ValueInputWidget* m_pWidth;
    ValueInputWidget* m_pHeight;
    ValueInputWidget* m_pX;
    ValueInputWidget* m_pY;
};


#endif